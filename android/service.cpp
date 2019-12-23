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

#include "service.h"
#include "mediaplayer.h"
#include "mediaplayerconfig.h"
#include "media.h"
#include "bookmarks.h"
#include <QAndroidBinder>
#include <android/log.h>
#include <QAndroidParcel>

#define applicationName "QuranMediaService"

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
  QString report = msg;
  if (context.file && !QString(context.file).isEmpty()) {
    report += " in file ";
    report += QString(context.file);
    report += " line ";
    report += QString::number(context.line);
  }

  if (context.function && !QString(context.function).isEmpty()) {
    report += " function ";
    report += QString(context.function);
  }

  const char *const local = report.toLocal8Bit().constData();
  switch (type) {
  case QtDebugMsg:
    __android_log_write(ANDROID_LOG_DEBUG, applicationName, local);
    break;
  case QtInfoMsg:
    __android_log_write(ANDROID_LOG_INFO, applicationName, local);
    break;
  case QtWarningMsg:
    __android_log_write(ANDROID_LOG_WARN, applicationName, local);
    break;
  case QtCriticalMsg:
    __android_log_write(ANDROID_LOG_ERROR, applicationName, local);
    break;
  case QtFatalMsg:
  default:
    __android_log_write(ANDROID_LOG_FATAL, applicationName, local);
    abort();
  }
}

class Binder : public QAndroidBinder {
public:
  Binder(MediaPlayer *player) :
    m_player(player) {

  }

  bool onTransact(int code, const QAndroidParcel& data, const QAndroidParcel& reply,
		  QAndroidBinder::CallType flags) {

    switch (code) {
    case Service::UpdateBinder:
      m_sender = data.readBinder();
      return true;

    case Service::ActionPlay: {
      QByteArray d(data.readData());

      MediaPlayerConfig config = MediaPlayerConfig::fromByteArray(d);

      bool res = QMetaObject::invokeMethod(m_player, "play", Qt::AutoConnection,
					   Q_ARG(MediaPlayerConfig, config));

      reply.writeVariant(res);
    }
      return true;

    case Service::ActionStop:
      reply.writeVariant(QMetaObject::invokeMethod(m_player, "stop", Qt::AutoConnection));
      return true;

    case Service::ActionPause:
      reply.writeVariant(QMetaObject::invokeMethod(m_player, "pause", Qt::AutoConnection));
      return true;

    case Service::ActionResume:
      reply.writeVariant(QMetaObject::invokeMethod(m_player, "resume", Qt::AutoConnection));
      return true;

    case Service::QueryPosition: {
      uint pos;
      QMetaObject::invokeMethod(m_player, "getPosition",
				Qt::BlockingQueuedConnection, Q_RETURN_ARG(uint, pos));
      reply.writeVariant(pos);
    }
      return true;

    case Service::QueryPlaying: {
      bool st = false;
      QMetaObject::invokeMethod(m_player, "isPlaying",
				Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, st));
      reply.writeVariant(st);
    }
      return true;

    case Service::QueryPaused: {
      bool st = false;
      QMetaObject::invokeMethod(m_player, "isPaused",
				Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, st));
      reply.writeVariant(st);
    }
      return true;

    default:
      break;
    }

    return false;
  }

  QAndroidBinder m_sender;
  MediaPlayer *m_player;
};

Service::Service(int& argc, char **argv) :
  QAndroidService(argc, argv),
  m_player(new MediaPlayer(this)),
  m_receiver(new Binder(m_player)),
  m_chapter(-1),
  m_verse(-1) {

  qInstallMessageHandler(messageHandler);

  QObject::connect(m_player, SIGNAL(playingChanged()), this, SLOT(playingChanged()));
  QObject::connect(m_player, SIGNAL(pausedChanged()), this, SLOT(pausedChanged()));
  QObject::connect(m_player, SIGNAL(positionChanged(int, int)), this, SLOT(positionChanged(int, int)));
  QObject::connect(m_player, SIGNAL(error()), this, SLOT(error()));

  qRegisterMetaType<MediaPlayerConfig>();
}

Service::~Service() {
  delete m_receiver;
  m_receiver = 0;

  m_player->stop();
  delete m_player;
  m_player = 0;
}

QAndroidBinder *Service::onBind(const QAndroidIntent& intent) {
  return m_receiver;
}

void Service::playingChanged() {
  send(ActionPlayingChanged, m_player->isPlaying());
}

void Service::pausedChanged() {
  send(ActionPausedChanged, m_player->isPaused());
}

void Service::positionChanged(int chapter, int verse) {
  m_chapter = chapter;
  m_verse = verse;

  send(ActionUpdatePosition, getPosition());
}

void Service::error() {
  send(ActionError, true);
}

void Service::send(int code, const QVariant& data) {
  QAndroidParcel sendData, replyData;
  sendData.writeVariant(data);

  m_receiver->m_sender.transact(code, sendData, &replyData);
}

uint Service::getPosition() {
  return Bookmarks::serialize(m_chapter, m_verse);
}
