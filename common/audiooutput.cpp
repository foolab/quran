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

#include "audiooutput.h"
#if defined(SLES)
#include "sles.h"
#elif defined(PULSE)
#include "pulse.h"
#elif defined(ALSA)
#include "alsa.h"
#elif defined(OBOE)
#include "oboe.h"
#endif
#include <QDebug>

#define TIMER_INTERVAL_MS      25

AudioOutput *AudioOutput::create(QObject *parent) {
#if defined(PULSE)
  AudioOutput *out = new Pulse(parent);
#elif defined(SLES)
  AudioOutput *out = new Sles(parent);
#elif defined(ALSA)
  AudioOutput *out = new Alsa(parent);
#elif defined(OBOE)
  AudioOutput *out = new Oboe(parent);
#endif

  if (!out->connect()) {
    delete out;
    return 0;
  }

  return out;
}

AudioOutput::AudioOutput(QObject *parent) :
  QObject(parent),
  m_paused(false) {

  m_timer.setSingleShot(false);
  m_timer.setInterval(TIMER_INTERVAL_MS);

  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

AudioOutput::~AudioOutput() {

}

void AudioOutput::addBuffer(const AudioBuffer& buffer) {
  m_buffers << buffer;
}

bool AudioOutput::isRunning() {
  return m_timer.isActive() || m_paused;
}

bool AudioOutput::start() {
  if (isRunning()) {
    return true;
  }

  m_timer.start();

  return true;
}

void AudioOutput::stop() {
  m_data.clear();
  m_timer.stop();
}

void AudioOutput::pause() {
  m_timer.stop();
  m_paused = true;
}

void AudioOutput::resume() {
  m_timer.start();
  m_paused = false;
}

void AudioOutput::timeout() {
  if (isPaused()) {
    return;
  }

  if (m_data.isEmpty()) {
    if (m_buffers.isEmpty()) {
      // TODO:
      return;
    }

    AudioBuffer buffer = m_buffers.takeFirst();
    if (buffer.media.isEos()) {
      if (!hasFrames()) {
	emit finished();
      } else {
	// Put it back because we will be called again.
	m_buffers << buffer;
      }
      return;
    } else if (buffer.media.isError()) {
      if (!hasFrames()) {
	qWarning() << "Received error buffer from decoder";
	emit error();
      } else {
	// Put it back because we will be called again.
	m_buffers << buffer;
      }
      return;
    }

    emit positionChanged(buffer.media.index());

    m_data = buffer.data;
  }

  if (!writeData(m_data)) {
    qWarning() << "Error writing data to device";
    emit error();
    return;
  }
}

int AudioOutput::numberOfBuffers() {
  return m_buffers.size();
}

bool AudioOutput::isPaused() {
  return m_paused;
}
