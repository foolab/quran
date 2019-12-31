/*
 * Copyright (c) 2017-2019 Mohammed Sameer <msameer@foolab.org>.
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

#include "audiopolicy.h"
#include "androidsupport.h"
#include <QPointer>

static QPointer<AudioPolicy> m_audio;

extern "C" void
Java_org_foolab_quran_MediaSupport_audioFocusAcquired(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  if (m_audio) {
    QMetaObject::invokeMethod(m_audio, "acquired", Qt::QueuedConnection);
  }
}

extern "C" void
Java_org_foolab_quran_MediaSupport_audioFocusDenied(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  if (m_audio) {
    QMetaObject::invokeMethod(m_audio, "denied", Qt::QueuedConnection);
  }
}

extern "C" void
Java_org_foolab_quran_MediaSupport_audioFocusLost(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  if (m_audio) {
    QMetaObject::invokeMethod(m_audio, "lost", Qt::QueuedConnection);
  }
}

extern "C" void
Java_org_foolab_quran_MediaSupport_audioFocusReleased(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  // We don't care
}

extern "C" void
Java_org_foolab_quran_MediaSupport_stopRequested(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  if (m_audio) {
    QMetaObject::invokeMethod(m_audio, "stop", Qt::QueuedConnection);
  }
}

AudioPolicy::AudioPolicy(QObject *parent) :
  QObject(parent),
  m_obj(QAndroidJniObject("org/foolab/quran/MediaSupport", "()V")) {

  m_audio = this;
}

AudioPolicy::~AudioPolicy() {

}

bool AudioPolicy::acquire() {
  m_obj.callMethod<void>("acquireAudioFocus");

  return true;
}

void AudioPolicy::release() {
  m_obj.callMethod<void>("releaseAudioFocus");
}
