/*
 * Copyright (c) 2019-2020 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mediaservice.h"
#include <QDebug>
#include <QAndroidBinder>
#include <QtAndroid>
#include <QAndroidServiceConnection>
#include <QAndroidIntent>
#include <QAndroidParcel>
#include "bookmarks.h"
#include "service.h"
#include "mediaplayerconfig.h"
#include "binder.h"
#include "intent.h"
#include "parcel.h"
#include <QAndroidJniExceptionCleaner>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

#define SERVICE_BIND_TIMEOUT_MS 250

class ServiceConnection : public QAndroidServiceConnection {
public:
  ServiceConnection(MediaService *service) :
    m_service(service),
    m_bound(false),
    m_valid(false) {

  }

  ~ServiceConnection() {
    unbind();
  }

  void onServiceConnected(const QString& name, const QAndroidBinder& serviceBinder) {
    Q_UNUSED(name);

    m_lock.lock();
    m_sender = serviceBinder;
    m_valid = true;
    m_lock.unlock();

    QMetaObject::invokeMethod(m_service, "binderUpdated", Qt::AutoConnection);
  }

  void onServiceDisconnected(const QString& name) {
    Q_UNUSED(name);

    m_lock.lock();
    m_sender = QAndroidBinder();
    m_valid = false;
    m_lock.unlock();

    QMetaObject::invokeMethod(m_service, "binderUpdated", Qt::AutoConnection);
  }

  bool send(int code, const QAndroidParcel& data, QAndroidParcel *reply) {
    QMutexLocker l(&m_lock);

    if (!m_valid) {
      return false;
    }

    return m_sender.transact(code, data, reply);
  }

  bool isValid() {
    QMutexLocker l(&m_lock);
    return m_valid;
  }

  void bind() {
    if (m_bound) {
      return;
    }

    m_bound = QtAndroid::bindService(QAndroidIntent(QtAndroid::androidActivity(), SERVICE),
				     *this, QtAndroid::BindFlag::AutoCreate);
    if (!m_bound) {
      QMetaObject::invokeMethod(m_service, "binderUpdated", Qt::QueuedConnection);
    }
  }

  void unbind() {
    if (!m_bound) {
      return;
    }

    QAndroidJniExceptionCleaner cleaner;
    QAndroidJniObject obj = QtAndroid::androidContext();
    obj.callMethod<void>("unbindService", "(Landroid/content/ServiceConnection;)V",
			 handle().object());
    m_bound = false;
  }

private:
  MediaService *m_service;
  QAndroidBinder m_sender;
  bool m_bound;
  bool m_valid;
  QMutex m_lock;
};

MediaService::MediaService(QObject *parent) :
  QObject(parent),
  m_binder(new Binder),
  m_connection(new ServiceConnection(this)),
  m_flipToPause(false) {

  m_binder->addHandler(Service::ActionStateChanged,
		       Binder::MethodInvoker(this, QLatin1String("stateChanged")));

  m_binder->addHandler(Service::ActionError,
		       Binder::MethodInvoker(this, QLatin1String("error")));

  m_binder->addHandler(Service::ActionUpdatePosition,
		       [this](const QAndroidParcel& data) {
			 uint pos = data.readVariant().value<uint>();
			 int chapter, verse;
			 Bookmarks::deserialize(pos, chapter, verse);
			 return QMetaObject::invokeMethod(this, "positionChanged",
							  Qt::QueuedConnection,
							  Q_ARG(int, chapter),
							  Q_ARG(int, verse));
		       });

  m_connection->bind();
}

MediaService::~MediaService() {
  m_connection->unbind();

  delete m_connection;
  m_connection = 0;

  delete m_binder;
  m_binder = 0;
}

bool MediaService::isAvailable() {
  return m_connection->isValid();
}

void MediaService::play(const MediaPlayerConfig& config) {
  QAndroidJniExceptionCleaner cleaner;

  QByteArray data = config.toByteArray();

  Intent intent(QtAndroid::androidActivity().object(), SERVICE);
  intent.putExtra(KEY_CONFIG, data);
  intent.putExtraString(KEY_RECITER, config.reciter());
  intent.setAction(ACTION_PLAY);

  sendIntent(intent);
}

Quran::PlaybackState MediaService::state() {
  return get(Service::QueryState).value<Quran::PlaybackState>();
}

void MediaService::stop() {
  Intent intent(QtAndroid::androidActivity().object(), SERVICE);
  intent.setAction(ACTION_STOP);
  sendIntent(intent);
}

void MediaService::pause() {
  Intent intent(QtAndroid::androidActivity().object(), SERVICE);
  intent.setAction(ACTION_PAUSE);
  sendIntent(intent);
}

void MediaService::resume() {
  Intent intent(QtAndroid::androidActivity().object(), SERVICE);
  intent.setAction(ACTION_RESUME);
  sendIntent(intent);
}

bool MediaService::send(int code) {
  QAndroidParcel sendData, replyData;

  if (!m_connection->send(code, sendData, &replyData)) {
    qWarning() << Q_FUNC_INFO << "Failed to send " << code;
    emit error();
    return false;
  }

  bool result = replyData.readVariant().value<bool>();

  if (!result) {
    qWarning() << "Result false sending " << code;
    emit error();
    return false;
  }

  return true;
}

QVariant MediaService::get(int code) {
  QAndroidParcel sendData, replyData;

  if (!m_connection->send(code, sendData, &replyData)) {
    qWarning() << Q_FUNC_INFO << "Failed to send " << code;
    //    emit error();
    return QVariant();
  }

  return replyData.readVariant();
}

void MediaService::binderUpdated() {
  emit isAvailableChanged();

  if (!m_connection->isValid()) {
    // reconnect
    QTimer::singleShot(SERVICE_BIND_TIMEOUT_MS,
		       [this] () {
			 m_connection->unbind();
			 m_connection->bind();
		       });
    emit error();
  } else {
    QAndroidParcel sendData, replyData;
    sendData.writeBinder(*m_binder);
    m_connection->send(Service::UpdateBinder, sendData, &replyData);
    syncSettingsToService();
  }
}

void MediaService::sendIntent(Intent& intent) {
  if (!intent.send()) {
    emit error();
  }
}

bool MediaService::isFlipToPauseEnabled() {
  return m_flipToPause;
}

void MediaService::setFlipToPauseEnabled(bool enabled) {
  if (m_flipToPause != enabled) {
    m_flipToPause = enabled;

    syncSettingsToService();

    emit flipToPauseChanged();
  }
}

void MediaService::syncSettingsToService() {
  Bundle bundle;
  bundle.setProperty(FLIP_TO_PAUSE, m_flipToPause);
  Parcel parcel;
  parcel.writeBundle(bundle);

  if (!m_connection->send(Service::UpdateSettings, parcel, 0)) {
    qWarning() << Q_FUNC_INFO << "Failed to update service settings";
    // We do not emit an error because we could be called because the flip to pause
    // setting is set to true and we are starting up
    //    emit error();
  }
}
