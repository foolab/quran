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

#include "intent.h"
#include <QAndroidJniEnvironment>
#include <QAndroidJniExceptionCleaner>
#include <QtAndroid>

Intent::Intent(QAndroidIntent& intent) :
  QAndroidIntent(intent.handle()) {

}

void Intent::putExtraString(const QString& key, const QString& data) {
  QAndroidJniExceptionCleaner cleaner;

  handle().callObjectMethod("putExtra",
			    "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
			    QAndroidJniObject::fromString(key).object(),
			    QAndroidJniObject::fromString(data).object());
}
#if 0
QString Intent::extraString(const QString& key) {
  QAndroidJniExceptionCleaner cleaner;

  return handle().callObjectMethod("getStringExtra",
				   "(Ljava/lang/String;)Ljava/lang/String;",
				   QAndroidJniObject::fromString(key).object()).toString();
}
#endif

void Intent::putExtraInt(const QString& key, int data) {
  QAndroidJniExceptionCleaner cleaner;

  handle().callObjectMethod("putExtra",
			    "(Ljava/lang/String;I)Landroid/content/Intent;",
			    QAndroidJniObject::fromString(key).object(),
			    data);
}

void Intent::setAction(const QString& action) {
  QAndroidJniExceptionCleaner cleaner;

  handle().callObjectMethod("setAction",
			    "(Ljava/lang/String;)Landroid/content/Intent;",
			    QAndroidJniObject::fromString(action).object());
}

QString Intent::action() {
  QAndroidJniExceptionCleaner cleaner;

  return handle().callObjectMethod("getAction",
				   "()Ljava/lang/String;").toString();
}

void Intent::setBundle(const QString& key, Bundle& bundle) {
  QAndroidJniExceptionCleaner cleaner;

  handle().callObjectMethod("putExtra",
			    "(Ljava/lang/String;Landroid/os/Bundle;)Landroid/content/Intent;",
			    QAndroidJniObject::fromString(key).object(),
			    bundle.handle().object());
}

Bundle Intent::bundle(const QString& key) {
  QAndroidJniExceptionCleaner cleaner;

  QAndroidJniObject bundle =
    handle().callObjectMethod("getBundleExtra",
			      "(Ljava/lang/String;)Landroid/os/Bundle;",
			      QAndroidJniObject::fromString(key).object());
  return Bundle(bundle);
}

bool Intent::send() {
  QAndroidJniExceptionCleaner cleaner;

  QAndroidJniObject obj = QtAndroid::androidContext()
    .callObjectMethod("startService", "(Landroid/content/Intent;)Landroid/content/ComponentName;",
		      handle().object());
  return obj.isValid();
}
