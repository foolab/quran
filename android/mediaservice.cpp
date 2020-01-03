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
#include <QAndroidJniExceptionCleaner>

class ServiceConnection : public QAndroidServiceConnection {
public:
  ServiceConnection(MediaService *service) :
    m_service(service) {

  }

  void onServiceConnected(const QString& name, const QAndroidBinder& serviceBinder) {
    Q_UNUSED(name);

    m_sender = serviceBinder;

    QMetaObject::invokeMethod(m_service, "binderUpdated", Qt::AutoConnection);
  }

  void onServiceDisconnected(const QString& name) {
    Q_UNUSED(name);

    QMetaObject::invokeMethod(m_service, "error", Qt::AutoConnection);
  }

  bool send(int code, const QAndroidParcel& data, QAndroidParcel *reply) {
    return m_sender.transact(code, data, reply);
  }

private:
  MediaService *m_service;
  QAndroidBinder m_sender;
};

MediaService::MediaService(QObject *parent) :
  QObject(parent),
  m_binder(new Binder),
  m_connection(new ServiceConnection(this)) {

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

  // TODO: unbind
  // TODO: error
  QtAndroid::bindService(QAndroidIntent(QtAndroid::androidActivity(), SERVICE),
			 *m_connection, QtAndroid::BindFlag::AutoCreate);
}

MediaService::~MediaService() {
  // TODO: unbind

  delete m_connection;
  m_connection = 0;

  delete m_binder;
  m_binder = 0;
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
  QAndroidParcel sendData, replyData;
  sendData.writeBinder(*m_binder);
  m_connection->send(Service::UpdateBinder, sendData, &replyData);
}

void MediaService::sendIntent(const Intent& intent) {
  QAndroidJniObject obj = QtAndroid::androidContext()
    .callObjectMethod("startService", "(Landroid/content/Intent;)Landroid/content/ComponentName;",
		      intent.handle().object());
  if (!obj.isValid()) {
    emit error();
  }
}
