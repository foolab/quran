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

#include "audiooutput.h"
#include "pulse.h"
#include <QDebug>

AudioOutput::AudioOutput(QObject *parent) :
  QObject(parent),
  m_media(0),
  m_pulse(0) {

}

AudioOutput::~AudioOutput() {
  stop();

  delete m_pulse;
}

void AudioOutput::queue(AudioBuffer buffer) {
  if (m_pulse) {
    m_pulse->play(buffer);
  }
  else {
    m_buffers << buffer;
  }
}

void AudioOutput::stop() {
  m_buffers.clear();

  if (m_pulse) {
    m_pulse->stop();
  }

  emit finished();
}

void AudioOutput::start() {
  if (!m_pulse) {
    m_pulse = new Pulse(this);

    QObject::connect(m_pulse, SIGNAL(finished()), this, SIGNAL(finished()));
    QObject::connect(m_pulse, SIGNAL(positionChanged(int)),
		     this, SIGNAL(positionChanged(int)));

    if (!m_pulse->connect()) {
      delete m_pulse;
      m_pulse = 0;

      emit error();
      return;
    }

    foreach (AudioBuffer buffer, m_buffers) {
      m_pulse->play(buffer);
    }

    m_buffers.clear();
  }
}
