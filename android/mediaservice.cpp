/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#define SERVICE "org.foolab.quran.MediaService"

class ServiceConnection : public QAndroidServiceConnection {
public:
  ServiceConnection(MediaService *service, Binder *binder) :
    m_service(service),
    m_localBinder(binder) {

  }

  void onServiceConnected(const QString& name, const QAndroidBinder& serviceBinder) {
    m_sender = serviceBinder;
    QAndroidParcel sendData, replyData;
    sendData.writeBinder(*m_localBinder);
    m_sender.transact(Service::UpdateBinder, sendData, &replyData);
  }

  void onServiceDisconnected(const QString& name) {
    QMetaObject::invokeMethod(m_service, "error", Qt::AutoConnection);
  }

  bool send(int code, const QAndroidParcel& data, QAndroidParcel *reply) {
    return m_sender.transact(code, data, reply);
  }

private:
  MediaService *m_service;
  QAndroidBinder m_sender;
  Binder *m_localBinder;
};

MediaService::MediaService(QObject *parent) :
  QObject(parent),
  m_binder(new Binder),
  m_connection(new ServiceConnection(this, m_binder)) {

  m_binder->addHandler(Service::ActionPlayingChanged,
		       Binder::MethodInvoker(this, QLatin1String("playingChanged")));

  m_binder->addHandler(Service::ActionPausedChanged,
		       Binder::MethodInvoker(this, QLatin1String("pausedChanged")));

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
  QByteArray data = config.toByteArray();
  if (!send(Service::ActionPlay, &data)) {
    emit error();
  }
}

bool MediaService::isPlaying() {
  return get(Service::QueryPlaying);
}

bool MediaService::isPaused() {
  return get(Service::QueryPaused);
}

void MediaService::stop() {
  send(Service::ActionStop);
}

void MediaService::pause() {
  send(Service::ActionPause);
}

void MediaService::resume() {
  send(Service::ActionResume);
}

bool MediaService::send(int code, const QByteArray *data) {
  QAndroidParcel sendData, replyData;
  if (data) {
    sendData.writeData(*data);
  }

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

bool MediaService::get(int code) {
  QAndroidParcel sendData, replyData;

  if (!m_connection->send(code, sendData, &replyData)) {
    qWarning() << Q_FUNC_INFO << "Failed to send " << code;
    //    emit error();
    return false;
  }

  bool result = replyData.readVariant().value<bool>();

  return result;
}