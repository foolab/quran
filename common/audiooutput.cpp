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
  m_pulse(0) {

}

AudioOutput::~AudioOutput() {
  stop();
}

void AudioOutput::stop() {
  if (m_pulse) {
    m_mutex.lock();
    m_buffers.clear();
    m_buffers << AudioBuffer(Media::eos());
    m_cond.wakeOne();
    m_mutex.unlock();
    m_pulse->stop();
    delete m_pulse;
    m_pulse = 0;
  }
}

bool AudioOutput::start() {
  if (!m_pulse) {
    m_pulse = new Pulse(this);
    QObject::connect(m_pulse, SIGNAL(positionChanged(int)),
		     this, SLOT(pulsePositionChanged(int)), Qt::QueuedConnection);

    QObject::connect(m_pulse, SIGNAL(finished()), this, SIGNAL(finished()));
    QObject::connect(m_pulse, SIGNAL(error()), this, SIGNAL(error()));

    if (!m_pulse->connect()) {
      delete m_pulse;
      m_pulse = 0;

      return false;
    }
  }

  return true;
}

void AudioOutput::play(const QList<AudioBuffer>& buffers) {
  m_mutex.lock();
  m_buffers = buffers;
  m_mutex.unlock();

  if (!m_buffers.isEmpty()) {
    m_pulse->start();
    m_cond.wakeOne();
  }
}

void AudioOutput::play(const AudioBuffer& buffer) {
  // TODO: limit the size of m_buffers

  m_mutex.lock();
  m_buffers << buffer;
  m_mutex.unlock();

  m_pulse->start();
  m_cond.wakeOne();
}

AudioBuffer AudioOutput::buffer() {
  QMutexLocker locker(&m_mutex);
  if (m_buffers.isEmpty()) {
    m_cond.wait(&m_mutex);
  }

  if (m_buffers.isEmpty()) {
    return AudioBuffer(Media::eos());
  }

  AudioBuffer b = m_buffers.takeFirst();

  if (b.media.isError()) {
    emit error();
    return AudioBuffer(Media::eos());
  }

  return b;
}

void AudioOutput::pulsePositionChanged(int index) {
  if (m_pulse) {
    emit positionChanged(index);
  }
}
