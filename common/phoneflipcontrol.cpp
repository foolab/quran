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

#include "phoneflipcontrol.h"
#include <QOrientationSensor>
#include <QOrientationReading>
#include "settings.h"
#include <QDebug>

PhoneFlipControl::PhoneFlipControl(Settings *settings, QObject *parent)
  : QObject(parent), m_settings(settings), m_sensor(new QOrientationSensor(this)), m_run(false) {

  QObject::connect(m_sensor, SIGNAL(readingChanged()), this, SLOT(sensorReadingChanged()));
  QObject::connect(m_settings, SIGNAL(flipToStopRecitationChanged()),
		   this, SLOT(flipToStopRecitationChanged()));

  flipToStopRecitationChanged();
}

PhoneFlipControl::~PhoneFlipControl() {
  m_sensor->stop();
}

void PhoneFlipControl::start() {
  m_run = true;

  flipToStopRecitationChanged();
}

void PhoneFlipControl::stop() {
  m_run = false;

  m_sensor->stop();
}

void PhoneFlipControl::flipToStopRecitationChanged() {
  if (m_run && m_settings->flipToStopRecitation()) {
    m_sensor->start();
  }
  else {
    m_sensor->stop();
  }
}

void PhoneFlipControl::sensorReadingChanged() {
  if (m_run && m_settings->flipToStopRecitation() &&
      m_sensor->reading()->orientation() == QOrientationReading::FaceDown) {
    emit flipped();
  }
}
