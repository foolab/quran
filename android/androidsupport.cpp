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
#include <QPointer>
#include "audiopolicy.h"
#include <android/asset_manager_jni.h>
#include "sqlite-ndk/sources/sqlite3ndk.h"

AAssetManager *m_assets = 0;
static jobject m_obj = 0;
static jclass m_class = 0;
static jmethodID m_portrait = 0;
static jmethodID m_landscape = 0;
static jmethodID m_unlock = 0;
static jmethodID m_acquire = 0;
static jmethodID m_release = 0;
QPointer<AudioPolicy> m_audio;

static void audioFocusAcquired(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  if (m_audio) {
    QMetaObject::invokeMethod(m_audio, "acquired", Qt::QueuedConnection);
  }
}

static void audioFocusDenied(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  if (m_audio) {
    QMetaObject::invokeMethod(m_audio, "denied", Qt::QueuedConnection);
  }
}

static void audioFocusLost(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  if (m_audio) {
    QMetaObject::invokeMethod(m_audio, "lost", Qt::QueuedConnection);
  }
}

static void audioFocusReleased(JNIEnv *env, jobject objectOrClass) {
  Q_UNUSED(env);
  Q_UNUSED(objectOrClass);

  // We don't care
}

static void storeAssetManager(JNIEnv *env, jobject objectOrClass, jobject assetManager) {
  Q_UNUSED(objectOrClass);

  m_assets = AAssetManager_fromJava(env, env->NewGlobalRef(assetManager));
  sqlite3_ndk_init(m_assets);
}

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

void AndroidSupport::acquireAudioFocus(AudioPolicy *audio) {
  QAndroidJniEnvironment env;
  m_audio = audio;

  env->CallVoidMethod(m_obj, m_acquire, NULL);
}

void AndroidSupport::releaseAudioFocus() {
  QAndroidJniEnvironment env;

  env->CallVoidMethod(m_obj, m_release, NULL);
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

    JNINativeMethod methods[] = {
      {"audioFocusAcquired", "()V", reinterpret_cast<void *>(audioFocusAcquired)},
      {"audioFocusDenied", "()V", reinterpret_cast<void *>(audioFocusDenied)},
      {"audioFocusLost", "()V", reinterpret_cast<void *>(audioFocusLost)},
      {"audioFocusReleased", "()V", reinterpret_cast<void *>(audioFocusReleased)},
      {"storeAssetManager", "(Landroid/content/res/AssetManager;)V", reinterpret_cast<void *>(storeAssetManager)},
    };

    if (env->RegisterNatives(m_class, methods,
			     sizeof(methods) / sizeof(methods[0])) != JNI_OK) {
      qCritical() << "Failed to register native methods";
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

    m_acquire = env->GetMethodID(m_class, "acquireAudioFocus", "()V");
    if (!m_acquire) {
      qCritical() << "Cannot find acquireAudioFocus";
      return -1;
    }

    m_release = env->GetMethodID(m_class, "releaseAudioFocus", "()V");
    if (!m_release) {
      qCritical() << "Cannot find releaseAudioFocus";
      return -1;
    }

    return JNI_VERSION_1_6;
  }
}
