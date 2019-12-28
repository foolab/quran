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

#include "binder.h"
#include <QAndroidParcel>
#include <QDebug>

Binder::Binder() {

}

Binder::~Binder() {

}

bool Binder::onTransact(int code, const QAndroidParcel& data, const QAndroidParcel& reply,
			QAndroidBinder::CallType flags) {

  Q_UNUSED(flags);

  if (m_handlers.contains(code)) {
    reply.writeVariant(m_handlers[code](data));
    return true;
  }

  qCritical() << "No handler for action " << code;

  return false;
}

void Binder::addHandler(int code, const BinderHandler& func) {
  m_handlers[code] = func;
}
