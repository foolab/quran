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

#include "intent.h"
#include <QAndroidJniEnvironment>
#include <QAndroidJniExceptionCleaner>

void Intent::putExtra(const QString& key, const QString& data) {
  QAndroidJniExceptionCleaner cleaner;

  handle().callObjectMethod("putExtra",
			    "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
			    QAndroidJniObject::fromString(key).object(),
			    QAndroidJniObject::fromString(data).object());
}

QString Intent::extraString(const QString& key) {
  QAndroidJniExceptionCleaner cleaner;

  return handle().callObjectMethod("getStringExtra",
				   "(Ljava/lang/String;)Ljava/lang/String;",
				   QAndroidJniObject::fromString(key).object()).toString();
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
