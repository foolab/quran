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

#include "parcel.h"
#include <QAndroidJniExceptionCleaner>
#include <QAndroidParcel>

Parcel::Parcel() :
  m_handle(QAndroidJniObject::callStaticObjectMethod("android/os/Parcel",
						     "obtain",
						     "()Landroid/os/Parcel;").object()) {

  QAndroidJniExceptionCleaner cleaner;
}

Parcel::Parcel(QAndroidParcel& parcel) :
  m_handle(parcel.handle()) {

}

Parcel::~Parcel() {

}

Bundle Parcel::readBundle() {
  QAndroidJniExceptionCleaner cleaner;

  QAndroidJniObject bundle = m_handle.callObjectMethod("readBundle", "()Landroid/os/Bundle;");
  return Bundle(bundle);
}

void Parcel::writeBundle(Bundle& bundle) {
  QAndroidJniExceptionCleaner cleaner;

  m_handle.callMethod<void>("writeBundle", "(Landroid/os/Bundle;)V",
			    bundle.handle().object());
}

QAndroidBinder Parcel::readBinder() {
  QAndroidJniExceptionCleaner cleaner;
  QAndroidJniObject binder = m_handle.callObjectMethod("readStrongBinder",
							 "()Landroid/os/IBinder;");
  return QAndroidBinder(binder.object());
}

void Parcel::writeBinder(QAndroidBinder& binder) {
  QAndroidJniExceptionCleaner cleaner;

  m_handle.callMethod<void>("writeStrongBinder",
			    "(Landroid/os/IBinder;)V",
			    binder.handle().object());
}

QAndroidJniObject Parcel::handle() {
  return m_handle;
}
