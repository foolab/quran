/*
 * Copyright (c) 2011-2014 Mohammed Sameer <msameer@foolab.org>.
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

#include "androidsupport.h"
#include <QtAndroid>
#include <QDebug>
#include <QAndroidJniEnvironment>

static jobject m_obj = 0;
static jclass m_class = 0;
static jmethodID m_portrait = 0;
static jmethodID m_landscape = 0;
static jmethodID m_unlock = 0;

AndroidSupport::AndroidSupport(QObject *parent) :
  QObject(parent),
  m_orientation(OrientationAll) {

}

AndroidSupport::~AndroidSupport() {

}

AndroidSupport::Orientation AndroidSupport::orientation() const {
  return m_orientation;
}

void AndroidSupport::setOrientation(const AndroidSupport::Orientation& orientation) {
  if (orientation != m_orientation) {
    m_orientation = orientation;

    applyOrientation();

    emit orientationChanged();
  }
}

void AndroidSupport::applyOrientation() {
  QAndroidJniEnvironment env;

  switch (m_orientation) {
  case OrientationAll:
    env->CallVoidMethod(m_obj, m_unlock, NULL);
    break;

  case OrientationPortrait:
    env->CallVoidMethod(m_obj, m_portrait, NULL);
    break;

  case OrientationLandscape:
    env->CallVoidMethod(m_obj, m_landscape, NULL);
    return;
  }
}

extern "C" {
  JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    Q_UNUSED(reserved);

    JNIEnv* env;

    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
      qCritical() << "Can't get JNI enviroument";
      return -1;
    }

    m_class = env->FindClass("org/foolab/quran/AndroidSupport");
    if (m_class == 0) {
      qCritical() << "AndroidSupport class not found";
      return -1;
    }

    jmethodID ctor = env->GetMethodID(m_class, "<init>", "()V");
    if (!ctor) {
      qCritical() << "Cannot fint AndroidSupport constructor";
      return -1;
    }

    jobject obj = env->NewObject(m_class, ctor);
    if (!obj) {
      qCritical() << "Failed to create AndroidSupport";
      return -1;
    }

    m_obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);

    m_portrait = env->GetMethodID(m_class, "lockOrientationPortrait", "()V");
    if (!m_portrait) {
      qCritical() << "Cannot find lockOrientationPortrait";
      return -1;
    }

    m_landscape = env->GetMethodID(m_class, "lockOrientationLandscape", "()V");
    if (!m_landscape) {
      qCritical() << "Cannot find lockOrientationLandscape";
      return -1;
    }

    m_unlock = env->GetMethodID(m_class, "unlockOrientation", "()V");
    if (!m_unlock) {
      qCritical() << "Cannot find unlockOrientation";
      return -1;
    }

    return JNI_VERSION_1_6;
  }
}
