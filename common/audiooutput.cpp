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

#include "audiooutput.h"
#if defined(SLES)
#include "sles.h"
#elif defined(PULSE)
#include "pulse.h"
#endif
#include <QDebug>

AudioOutput::AudioOutput(QObject *parent) :
  QObject(parent),
  m_out(0) {

}

AudioOutput::~AudioOutput() {
  stop();
}

void AudioOutput::stop() {
  if (m_out) {
    m_mutex.lock();
    m_buffers.clear();
    m_buffers << AudioBuffer(Media::eos());
    m_cond.wakeOne();
    m_mutex.unlock();
    m_out->stop();
    delete m_out;
    m_out = 0;
  }
}

bool AudioOutput::start() {
  if (!m_out) {
#ifndef ANDROID
    m_out = new Pulse(this);
#else
    m_out = new Sles(this);
#endif
    QObject::connect(m_out, SIGNAL(positionChanged(int)),
		     this, SLOT(audioPositionChanged(int)), Qt::QueuedConnection);

    QObject::connect(m_out, SIGNAL(finished()), this, SIGNAL(finished()));
    QObject::connect(m_out, SIGNAL(error()), this, SIGNAL(error()));

    if (!m_out->connect()) {
      delete m_out;
      m_out = 0;

      return false;
    }
  }

  return true;
}

void AudioOutput::play(const AudioBuffer& buffer) {
  // TODO: limit the size of m_buffers

  m_mutex.lock();
  m_buffers << buffer;

  if (m_out->isRunning()) {
    m_cond.wakeOne();
    m_mutex.unlock();
  }
  else {
    m_mutex.unlock();
    m_out->start();
  }
}

AudioBuffer AudioOutput::buffer() {
  m_mutex.lock();

  if (m_buffers.isEmpty()) {
    m_cond.wait(&m_mutex);
  }

  if (m_buffers.isEmpty()) {
    m_mutex.unlock();
    return AudioBuffer(Media::eos());
  }

  AudioBuffer b = m_buffers.takeFirst();
  m_mutex.unlock();

  return b;
}

void AudioOutput::audioPositionChanged(int index) {
  if (m_out) {
    emit positionChanged(index);
  }
}
