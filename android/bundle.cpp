/*
 * Copyright (c) 2020 Mohammed Sameer <msameer@foolab.org>.
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

#include "bundle.h"
#include <QAndroidJniExceptionCleaner>

Bundle::Bundle() :
  m_handle("android/os/Bundle", "()V") {

  QAndroidJniExceptionCleaner cleaner;
}

Bundle::Bundle(QAndroidJniObject& handle) :
  m_handle(handle) {

}

Bundle::~Bundle() {

}

void Bundle::setProperty(const QString& key, bool value) {
  QAndroidJniExceptionCleaner cleaner;
  m_handle.callMethod<void>("putBoolean", "(Ljava/lang/String;Z)V",
			    QAndroidJniObject::fromString(key).object(), value);
}

bool Bundle::isPropertySet(const QString& key) {
  QAndroidJniExceptionCleaner cleaner;
  return m_handle.callMethod<jboolean>("containsKey", "(Ljava/lang/String;)Z",
				       QAndroidJniObject::fromString(key).object());
}

bool Bundle::property(const QString& key) {
  QAndroidJniExceptionCleaner cleaner;
  return m_handle.callMethod<jboolean>("getBoolean", "(Ljava/lang/String;)Z",
				       QAndroidJniObject::fromString(key).object());
}

bool Bundle::property(const QString& key, bool defaultValue) {
  QAndroidJniExceptionCleaner cleaner;

  return m_handle.callMethod<jboolean>("getBoolean", "(Ljava/lang/String;Z)Z",
				       QAndroidJniObject::fromString(key).object(), defaultValue);
}

QAndroidJniObject Bundle::handle() {
  return m_handle;
}
