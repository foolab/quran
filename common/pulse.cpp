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

#include "pulse.h"
#include "audiooutput.h"
#include <QDebug>
#include "media.h"

Pulse::Pulse(QObject *parent) :
  QObject(parent),
  m_loop(0),
  m_ctx(0),
  m_stream(0) {

  m_loop = pa_threaded_mainloop_new();
}

Pulse::~Pulse() {
  stop();

  if (m_ctx) {
    pa_context_disconnect(m_ctx);
    pa_context_unref(m_ctx);
    m_ctx = 0;
  }

  pa_threaded_mainloop_free(m_loop);
  m_loop = 0;
}

void Pulse::stop() {
  if (m_stream) {
    pa_threaded_mainloop_lock(m_loop);
    pa_operation *o = pa_stream_flush(m_stream, (pa_stream_success_cb_t)successCallback, this);

    if (o) {
      while (pa_operation_get_state(o) == PA_OPERATION_RUNNING) {
	pa_threaded_mainloop_wait(m_loop);
      }

      pa_operation_unref(o);
    }

    pa_threaded_mainloop_unlock(m_loop);

    pa_stream_disconnect(m_stream);
    pa_stream_unref(m_stream);
    m_stream = 0;
  }

  pa_threaded_mainloop_stop(m_loop);

  m_buffers.clear();
}

bool Pulse::connect() {
  if (m_ctx) {
    return true;
  }

  m_ctx = pa_context_new(pa_threaded_mainloop_get_api(m_loop), "Quran");
  if (!m_ctx) {
    return false;
  }

  pa_context_set_state_callback(m_ctx, (pa_context_notify_cb_t)contextStateCallback, this);

  if (pa_context_connect(m_ctx, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL) < 0) {
    qDebug() << pa_context_errno(m_ctx);
    pa_context_unref(m_ctx);
    m_ctx = 0;
    return false;
  }

  pa_threaded_mainloop_lock(m_loop);
  if (pa_threaded_mainloop_start(m_loop) < 0) {
    pa_threaded_mainloop_unlock(m_loop);
    pa_context_disconnect(m_ctx);
    pa_context_unref(m_ctx);
    m_ctx = 0;

    return false;
  }

  while (true) {
    bool out = false;

    switch (pa_context_get_state(m_ctx)) {
    case PA_CONTEXT_UNCONNECTED:
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
      pa_threaded_mainloop_wait(m_loop);
      continue;

    case PA_CONTEXT_READY:
      out = true;
      break;

    case PA_CONTEXT_FAILED:
    case PA_CONTEXT_TERMINATED:
      pa_threaded_mainloop_unlock(m_loop);
      pa_context_disconnect(m_ctx);
      pa_context_unref(m_ctx);
      m_ctx = 0;
      return false;
    }

    if (out) {
      break;
    }
  }

  pa_context_set_state_callback(m_ctx, NULL, NULL);

  pa_threaded_mainloop_unlock(m_loop);

  return true;
}

void Pulse::contextStateCallback(pa_context *ctx, Pulse *pulse) {
  Q_UNUSED(ctx);

  pa_threaded_mainloop_signal(pulse->m_loop, 0);
}

void Pulse::streamStateCallback(pa_stream *stream, Pulse *pulse) {
  Q_UNUSED(stream);

  pa_threaded_mainloop_signal(pulse->m_loop, 0);
}

void Pulse::streamWriteCallback(pa_stream *stream, size_t length, Pulse *pulse) {
  Q_UNUSED(stream);
  Q_UNUSED(length);

  pulse->writeData();
}

void Pulse::successCallback(pa_stream *stream, int success, Pulse *pulse) {
  Q_UNUSED(stream);
  Q_UNUSED(success);

  pa_threaded_mainloop_signal(pulse->m_loop, 0);
}

bool Pulse::play(AudioBuffer& buffer) {
  m_mutex.lock();
  m_buffers << buffer;
  m_mutex.unlock();

  if (!m_ctx) {
    return false;
  }

  if (m_stream) {
    const pa_sample_spec *ss = pa_stream_get_sample_spec(m_stream);
    if (ss->channels != buffer.channels || ss->rate != buffer.rate) {
      return false;
    }

    return true;
  }

  pa_sample_spec ss;
  ss.format = PA_SAMPLE_S16NE;
  ss.channels = buffer.channels;
  ss.rate = buffer.rate;
  m_stream = pa_stream_new(m_ctx, "Quran", &ss, NULL);

  if (!m_stream) {
    return false;
  }

  pa_stream_set_state_callback(m_stream, (pa_stream_notify_cb_t)streamStateCallback, this);
  pa_stream_set_write_callback(m_stream, (pa_stream_request_cb_t)streamWriteCallback, this);
  pa_threaded_mainloop_lock(m_loop);
  if (pa_stream_connect_playback(m_stream, NULL, NULL, PA_STREAM_NOFLAGS, NULL, NULL) < 0) {
    qDebug() << pa_context_errno(m_ctx);
    pa_threaded_mainloop_unlock(m_loop);
    return false;
  }

  while (true) {
    bool out = false;

    switch (pa_stream_get_state(m_stream)) {
    case PA_STREAM_FAILED:
    case PA_STREAM_TERMINATED:
      pa_stream_disconnect(m_stream);
      pa_stream_unref(m_stream);
      pa_threaded_mainloop_unlock(m_loop);
      return false;

    case PA_STREAM_READY:
      pa_threaded_mainloop_unlock(m_loop);
      out = true;
      break;

    case PA_STREAM_UNCONNECTED:
    case PA_STREAM_CREATING:
      pa_threaded_mainloop_wait(m_loop);
      continue;
    }

    if (out) {
      break;
    }
  }

  // We have a stream at last.

  return true;
}

void Pulse::writeData() {
  // Called from another thread
  m_mutex.lock();
  if (m_buffers.isEmpty()) {
    // TODO:
    m_mutex.unlock();
    return;
  }

  AudioBuffer buffer = m_buffers.takeFirst();
  m_mutex.unlock();

  if (buffer.data.isEmpty()) {
    // We are done.
    // Drain:
    QMetaObject::invokeMethod(this, "drainAndFinish", Qt::QueuedConnection);
    return;
  }

  emit positionChanged(buffer.media->index());

  while (!buffer.data.isEmpty()) {
    void *data;
    size_t len = buffer.data.size();
    if (pa_stream_begin_write(m_stream, &data, &len) < 0) {
      qDebug() << pa_context_errno(m_ctx);
      emit error();
      return;
    }

    len = qMin(len, (size_t)buffer.data.size());
    memcpy(data, buffer.data.constData(), len);
    buffer.data.remove(0, len);
    if (pa_stream_write(m_stream, data, len, NULL, 0, PA_SEEK_RELATIVE) < 0) {
      qDebug() << pa_context_errno(m_ctx);
      emit error();
      return;
    }
  }
}

void Pulse::drainAndFinish() {
  pa_threaded_mainloop_lock(m_loop);

  if (!m_stream) {
    pa_threaded_mainloop_unlock(m_loop);
    return;
  }

  pa_operation *o = pa_stream_drain(m_stream, (pa_stream_success_cb_t)successCallback, this);

  if (o) {
    while (pa_operation_get_state(o) == PA_OPERATION_RUNNING) {
      pa_threaded_mainloop_wait(m_loop);
    }

    pa_operation_unref(o);
  }

  pa_threaded_mainloop_unlock(m_loop);

  emit finished();
}
