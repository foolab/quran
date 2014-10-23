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

#include "orientation.h"
#include <QtAndroid>

// Taken from ActivityInfo
#define SCREEN_ORIENTATION_UNSPECIFIED  -1
#define SCREEN_ORIENTATION_LANDSCAPE    0
#define SCREEN_ORIENTATION_PORTRAIT     1

Orientation::Orientation(QObject *parent) :
  QObject(parent),
  m_orientation(OrientationAll) {

}

Orientation::~Orientation() {

}

Orientation::OrientationDirection Orientation::orientation() const {
  return m_orientation;
}

void Orientation::setOrientation(const Orientation::OrientationDirection& orientation) {
  if (orientation != m_orientation) {
    m_orientation = orientation;

    applyOrientation();

    emit orientationChanged();
  }
}

void Orientation::applyOrientation() {
  QAndroidJniObject activity = QtAndroid::androidActivity();
  activity.callMethod<void>("setRequestedOrientation", "(I)V", (jint)m_orientation);
}
