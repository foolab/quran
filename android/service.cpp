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
#include "binder.h"
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

Service::Service(int& argc, char **argv) :
  QAndroidService(argc, argv),
  m_player(new MediaPlayer(this)),
  m_localBinder(new Binder),
  m_chapter(-1),
  m_verse(-1) {

  qInstallMessageHandler(messageHandler);

  QObject::connect(m_player, SIGNAL(playingChanged()), this, SLOT(playingChanged()));
  QObject::connect(m_player, SIGNAL(pausedChanged()), this, SLOT(pausedChanged()));
  QObject::connect(m_player, SIGNAL(positionChanged(int, int)), this, SLOT(positionChanged(int, int)));
  QObject::connect(m_player, SIGNAL(error()), this, SLOT(error()));

  qRegisterMetaType<MediaPlayerConfig>();

  m_localBinder->addHandler(Service::UpdateBinder,
			    [this](const QAndroidParcel& data) {
			      m_sender = data.readBinder();
			      return true;
			    });

  m_localBinder->addHandler(Service::ActionPlay,
			    [this](const QAndroidParcel& data) {
			      QByteArray d(data.readData());
			      MediaPlayerConfig config = MediaPlayerConfig::fromByteArray(d);
			      return QMetaObject::invokeMethod(m_player, "play",
							       Qt::QueuedConnection,
							       Q_ARG(MediaPlayerConfig, config));
			    });
  m_localBinder->addHandler(Service::ActionStop,
			    Binder::MethodInvoker(m_player, QLatin1String("stop")));

  m_localBinder->addHandler(Service::ActionPause,
			    Binder::MethodInvoker(m_player, QLatin1String("pause")));

  m_localBinder->addHandler(Service::ActionResume,
			    Binder::MethodInvoker(m_player, QLatin1String("resume")));

  m_localBinder->addHandler(Service::QueryPosition,
			    Binder::UnsignedIntPropertyGetter(m_player, QLatin1String("getPosition")));

  m_localBinder->addHandler(Service::QueryPlaying,
			    Binder::BoolPropertyGetter(m_player, QLatin1String("isPlaying")));

  m_localBinder->addHandler(Service::QueryPaused,
			    Binder::BoolPropertyGetter(m_player, QLatin1String("isPaused")));
}

Service::~Service() {
  delete m_localBinder;
  m_localBinder = 0;

  m_player->stop();
  delete m_player;
  m_player = 0;
}

QAndroidBinder *Service::onBind(const QAndroidIntent& intent) {
  Q_UNUSED(intent);
  return m_localBinder;
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

  // TODO: This can disappear while we are manipulating it?
  m_sender.transact(code, sendData, &replyData);
}

uint Service::getPosition() {
  return Bookmarks::serialize(m_chapter, m_verse);
}
