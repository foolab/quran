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
#include "audiopolicy.h"
#include <pulse/error.h>

AudioOutput::AudioOutput(QObject *parent) :
  QObject(parent),
  m_policy(0),
  m_finish(false),
  m_simple(0),
  m_index(-1) {

  m_spec.format = PA_SAMPLE_S16NE;
  m_spec.channels = 0;
  m_spec.rate = 0;
}

AudioOutput::~AudioOutput() {
  qDeleteAll(m_buffers);

  if (m_simple) {
    pa_simple_drain (m_simple, NULL);
    pa_simple_free(m_simple);
  }
}

void AudioOutput::finish() {
  m_mutex.lock();
  m_finish = true;
  m_mutex.unlock();
}

void AudioOutput::process() {
  m_acquired = false;

  m_policy = new AudioPolicy(this);

  QObject::connect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
  QObject::connect(m_policy, SIGNAL(lost()), this, SLOT(policyLost()));
  QObject::connect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));

  if (!m_policy->acquire()) {
    emit error();
  }
}

void AudioOutput::policyAcquired() {
  m_acquired = true;

  if (!m_buffers.isEmpty()) {
    QMetaObject::invokeMethod(this, "playNext", Qt::QueuedConnection);
  }
}

void AudioOutput::policyDenied() {
  m_acquired = false;
  emit error();
}

void AudioOutput::policyLost() {
  m_acquired = false;
  emit error();
}

void AudioOutput::play(AudioBuffer *buffer) {
  m_buffers << buffer;

  if (m_acquired) {
    QMetaObject::invokeMethod(this, "playNext", Qt::QueuedConnection);
  }
}

void AudioOutput::playNext() {
  if (m_buffers.isEmpty()) {
    if (m_simple) {
      pa_simple_drain (m_simple, NULL);
    }

    emit finished();
    return;
  }

  AudioBuffer *b = m_buffers.takeAt(0);

  pa_sample_spec ss;
  ss.format = PA_SAMPLE_S16NE;
  ss.channels = b->channels;
  ss.rate = b->rate;

  if (m_spec.channels != ss.channels || m_spec.rate != ss.rate) {
    if (m_simple) {
      // Wait for playback
      pa_simple_drain (m_simple, NULL);
      // Destroy:
      pa_simple_free(m_simple);
      m_simple = 0;
    }
  }

  m_spec.rate = ss.rate;
  m_spec.channels = ss.channels;

  int err;
  if (!m_simple) {
    m_simple = pa_simple_new(NULL,               // Use the default server.
			     "Quran",           // Our application's name.
			     PA_STREAM_PLAYBACK,
			     NULL,               // Use the default device.
			     "Recitation",            // Description of our stream.
			     &ss,                // Our sample format.
			     NULL,               // Use default channel map
			     NULL,               // Use default buffering attributes.
			     &err                // Ignore error code.
			     );
  }

  if (!m_simple) {
    qWarning() << "Error connecting to pulse audio" << pa_strerror(err);
    emit error();
    delete b;
    return;
  }

  pa_simple_write(m_simple, b->data.constData(),
		       b->data.size(), NULL);

  if (b->index != m_index) {
    m_index = b->index;

    emit positionChanged(b->chapter, b->verse, b->index);
  }

  delete b;

  QMetaObject::invokeMethod(this, "playNext", Qt::QueuedConnection);
}
