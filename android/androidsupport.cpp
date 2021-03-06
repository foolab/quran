/*
 * Copyright (c) 2011-2020 Mohammed Sameer <msameer@foolab.org>.
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
#include <QAndroidJniExceptionCleaner>
#include <QPointer>
#include <android/asset_manager_jni.h>
#include "sqlite-ndk/sources/sqlite3ndk.h"

static AAssetManager *m_assets = 0;
static QPointer<AndroidSupport> m_support;

extern "C" void
Java_org_foolab_quran_AndroidSupport_storeAssetManager(JNIEnv *env,
						       jobject objectOrClass,
						       jobject assetManager) {
  Q_UNUSED(objectOrClass);

  m_assets = AAssetManager_fromJava(env, env->NewGlobalRef(assetManager));
  sqlite3_ndk_init(m_assets);
}

AndroidSupport::AndroidSupport(QObject *parent) :
  QObject(parent),
  m_orientation(OrientationAll),
  m_obj(QAndroidJniObject("org/foolab/quran/AndroidSupport", "()V")) {

  QAndroidJniExceptionCleaner cleaner;

  m_support = this;
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
  QAndroidJniExceptionCleaner cleaner;

  switch (m_orientation) {
  case OrientationAll:
    m_obj.callMethod<void>("unlockOrientation");
    break;

  case OrientationPortrait:
    m_obj.callMethod<void>("lockOrientationPortrait");
    break;

  case OrientationLandscape:
    m_obj.callMethod<void>("lockOrientationLandscape");
    return;
  }
}
