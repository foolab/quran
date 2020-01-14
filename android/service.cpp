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

#include "service.h"
#include "mediaplayer.h"
#include "mediaplayerconfig.h"
#include "mediastate.h"
#include "bookmarks.h"
#include "binder.h"
#include "intent.h"
#include "parcel.h"
#include <android/log.h>
#include <QAndroidParcel>
#include <QAndroidJniObject>
#include <QAndroidJniExceptionCleaner>
#include <QtAndroid>
#include <QDebug>
#include "flipsensor.h"

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

static Service *that = 0;

extern "C" jboolean
Java_org_foolab_quran_MediaService_onStartCommand(JNIEnv *env, jobject objectOrClass,
						  jobject i, jboolean restore) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  Intent intent = Intent(QAndroidJniObject(i));
  return that->onStartCommand(intent, restore);
}

Service::Service(int& argc, char **argv) :
  QAndroidService(argc, argv),
  m_state(new MediaState),
  m_player(new MediaPlayer(m_state, this)),
  m_localBinder(new Binder),
  m_sensor(new FlipSensor(this)),
  m_chapter(-1),
  m_verse(-1),
  m_flipToPause(false) {

  qInstallMessageHandler(messageHandler);

  qRegisterMetaType<MediaPlayerConfig>();
  qRegisterMetaTypeStreamOperators<Quran::PlaybackState>("Quran::PlaybackState");

  QObject::connect(m_player, SIGNAL(stateChanged()), this, SLOT(stateChanged()));
  QObject::connect(m_player, SIGNAL(positionChanged(int, int)), this, SLOT(positionChanged(int, int)));
  QObject::connect(m_player, SIGNAL(error()), this, SLOT(error()));
  QObject::connect(m_sensor, SIGNAL(flipped()), this, SLOT(pause()));

  m_localBinder->addHandler(Service::UpdateBinder,
			    [this](const QAndroidParcel& data) {
			      m_sender = data.readBinder();
			      return
				QMetaObject::invokeMethod(this, "sendState", Qt::QueuedConnection);
			    });

  m_localBinder->addHandler(Service::QueryState,
			    [this](const QAndroidParcel& data) {
			      Q_UNUSED(data);
			      Quran::PlaybackState st;
			      bool res =
				QMetaObject::invokeMethod(m_player, "state",
							  Qt::BlockingQueuedConnection,
							  Q_RETURN_ARG(Quran::PlaybackState, st));
			      Q_ASSERT_X(res, "QueryState", "Should not be triggered");
			      return QVariant::fromValue<Quran::PlaybackState>(st);
			    });

  m_localBinder->addHandler(Service::QueryPosition,
			    Binder::UnsignedIntPropertyGetter(m_player,
							      QLatin1String("getPosition")));

  m_localBinder->addHandler(Service::UpdateSettings,
			    [this](const QAndroidParcel& data) {
			      return QMetaObject::invokeMethod(this, "updateSettings",
							       Qt::BlockingQueuedConnection,
							       Q_ARG(QAndroidParcel, data));
			    });

  that = this;
}

Service::~Service() {
  delete m_localBinder;
  m_localBinder = 0;

  m_player->stop();
  delete m_player;
  m_player = 0;

  delete m_state;
  m_state = 0;

  delete m_sensor;
  m_sensor = 0;

  that = 0;
}

QAndroidBinder *Service::onBind(const QAndroidIntent& intent) {
  Q_UNUSED(intent);
  return m_localBinder;
}

void Service::stateChanged() {
  Quran::PlaybackState state = m_player->state();
  send(ActionStateChanged, QVariant::fromValue<Quran::PlaybackState>(state));
  if (state == Quran::Stopped) {
    stopService();
  } else {
    setSensorState();
  }
}

void Service::positionChanged(int chapter, int verse) {
  if (m_chapter != chapter || m_verse != verse) {
    m_chapter = chapter;
    m_verse = verse;

    send(ActionUpdatePosition, getPosition());
  }
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

bool Service::onStartCommand(Intent& intent, bool restore) {
  if (restore) {
    MediaPlayerConfig config = m_state->config();
    if (!config.isValid()) {
      qWarning() << "Failed to restore previous intent";
      // Assume action is stop
      intent.setAction(ACTION_STOP);
    } else {
      // Let's restore:
      QByteArray data = config.toByteArray();

      intent.putExtra(KEY_CONFIG, data);
      intent.putExtraString(KEY_RECITER, config.reciter());

      // TODO: save the state and restore it too.
      intent.setAction(ACTION_PAUSE);
      // TODO: position?

      qWarning() << "Restored config";
    }
  }

  QString action(intent.action());
  if (!restore && action == ACTION_STOP) {
    m_state->clear();
  }

  if (action == ACTION_PLAY) {
    QByteArray d(intent.extraBytes(KEY_CONFIG));
    MediaPlayerConfig config = MediaPlayerConfig::fromByteArray(d);
    return QMetaObject::invokeMethod(m_player, "play",
				     Qt::QueuedConnection,
				     Q_ARG(MediaPlayerConfig, config));
  } else if (!action.isEmpty()) {
    bool res =
      QMetaObject::invokeMethod(m_player, action.toUtf8().constData(), Qt::QueuedConnection);
    return action == ACTION_STOP ? false : res;
  } else {
    return false;
  }
}

uint Service::getPosition() {
  return Bookmarks::serialize(m_chapter, m_verse);
}

void Service::stopService() {
  QAndroidJniExceptionCleaner cleaner;
  QtAndroid::androidContext().callMethod<void>("_stopService");

  m_chapter = -1;
  m_verse = -1;
}

void Service::sendState() {
  send(ActionStateChanged, QVariant::fromValue<Quran::PlaybackState>(m_player->state()));
  send(ActionUpdatePosition, getPosition());
}

bool Service::updateSettings(const QAndroidParcel& data) {
  Parcel parcel(data);
  Bundle bundle = parcel.readBundle();

  Q_ASSERT(bundle.isPropertySet(FLIP_TO_PAUSE));

  m_flipToPause = bundle.property(FLIP_TO_PAUSE);

  setSensorState();

  return true;
}

void Service::setSensorState() {
  if (m_flipToPause && m_player->state() == Quran::Playing) {
    m_sensor->setActive(true);
  } else {
    m_sensor->setActive(false);
  }
}

void Service::pause() {
  Intent intent(QtAndroid::androidContext().object(), SERVICE);
  intent.setAction(ACTION_PAUSE);
  // TODO: error
  intent.send();
}
