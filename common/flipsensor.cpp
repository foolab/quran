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

#include "flipsensor.h"
#include <QOrientationSensor>
#include <QOrientationReading>
#include "settings.h"
#include <QDebug>

FlipSensor::FlipSensor(QObject *parent)
  : QObject(parent),
    m_sensor(new QOrientationSensor(this)) {

  QObject::connect(m_sensor, &QOrientationSensor::readingChanged,
		   [this]() {
		     if (m_sensor->reading()->orientation()
			 == QOrientationReading::FaceDown) {
		       emit flipped();
		     }
		   });
}

FlipSensor::~FlipSensor() {
  m_sensor->stop();
}

bool FlipSensor::isActive() const {
  return m_sensor->isActive();
}

void FlipSensor::setActive(bool active) {
  if (active != isActive()) {
    m_sensor->setActive(active);
    emit activeChanged();
  }
}
