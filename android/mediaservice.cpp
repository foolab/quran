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


#define SERVICE "org.foolab.quran.MediaService"

class ClientBinder : public QAndroidBinder {
public:
  ClientBinder(MediaService *service) :
    m_service(service) {

  }

  bool onTransact(int code, const QAndroidParcel& data, const QAndroidParcel& reply,
		  QAndroidBinder::CallType flags) {

    switch (code) {
    case Service::ActionPlayingChanged:
      reply.writeVariant(QMetaObject::invokeMethod(m_service, "playingChanged",
						   Qt::AutoConnection));
      return true;

    case Service::ActionPausedChanged:
      reply.writeVariant(QMetaObject::invokeMethod(m_service, "pausedChanged",
						   Qt::AutoConnection));
      return true;

    case Service::ActionUpdatePosition: {
      uint pos = data.readVariant().value<uint>();
      int chapter, verse;
      Bookmarks::deserialize(pos, chapter, verse);
      reply.writeVariant(QMetaObject::invokeMethod(m_service, "positionChanged",
						   Qt::AutoConnection,
						   Q_ARG(int, chapter),
						   Q_ARG(int, verse)));
    }
      return true;
    case Service::ActionError:
      reply.writeVariant(QMetaObject::invokeMethod(m_service, "error",
						   Qt::AutoConnection));
      return true;

    default:
      break;
    }

    return false;
  }

  MediaService *m_service;
};

class ServiceConnection : public QAndroidServiceConnection {
public:
  ServiceConnection(MediaService *service) :
    m_service(service),
    m_receiver(ClientBinder(service)) {

  }

  void onServiceConnected(const QString& name, const QAndroidBinder& serviceBinder) {
    m_sender = serviceBinder;
    QAndroidParcel sendData, replyData;
    sendData.writeBinder(m_receiver);
    m_sender.transact(Service::UpdateBinder, sendData, &replyData);
  }

  void onServiceDisconnected(const QString& name) {
    QMetaObject::invokeMethod(m_service, "error", Qt::AutoConnection);
  }

  MediaService *m_service;
  QAndroidBinder m_sender;
  ClientBinder m_receiver;
};

MediaService::MediaService(QObject *parent) :
  QObject(parent),
  m_connection(new ServiceConnection(this)) {
  // TODO: unbind
  // TODO: error
  // TODO: if playing then get status and update our ptoperties
  QtAndroid::bindService(QAndroidIntent(QtAndroid::androidActivity(), SERVICE),
			 *m_connection, QtAndroid::BindFlag::AutoCreate);
}

MediaService::~MediaService() {
  delete m_connection;
  m_connection = 0;
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

  if (!m_connection->m_sender.transact(code, sendData, &replyData)) {
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

  if (!m_connection->m_sender.transact(code, sendData, &replyData)) {
    qWarning() << Q_FUNC_INFO << "Failed to send " << code;
    //    emit error();
    return false;
  }

  bool result = replyData.readVariant().value<bool>();

  return result;
}
