/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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

#include "keyfilter.h"
#include <QGuiApplication>
#include <QKeyEvent>

KeyFilter::KeyFilter(QObject *parent) :
  QObject(parent) {
  QGuiApplication::instance()->installEventFilter(this);
}

KeyFilter::~KeyFilter() {
  QGuiApplication::instance()->removeEventFilter(this);
}

bool KeyFilter::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::KeyRelease) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->key() == Qt::Key_Back || keyEvent->key() == Qt::Key_Backspace) {
      emit backTriggered();
      return true;
    }
  }

  return QObject::eventFilter(watched, event);
}
