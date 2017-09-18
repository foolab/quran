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

#include "alsa.h"
#include <alsa/asoundlib.h>
#include <QDebug>

class Device {
public:
  Device() :
    m_handle(0) {
  }

  ~Device() {
    stop();
  }

  bool open() {
    int err = snd_pcm_open(&m_handle, "default", SND_PCM_STREAM_PLAYBACK, 0 /*SND_PCM_NONBLOCK*/);
    if (err < 0) {
      qWarning() << "Failed to open sound device:" << snd_strerror(err);
      return false;
    }

    return true;
  }

  bool init() {
    int err = snd_pcm_prepare(m_handle);
    if (err < 0) {
      qWarning() << "Cannot prepare audio device:" << snd_strerror(err);
      return false;
    }

    return true;
  }

  snd_pcm_t *handle() {
    return m_handle;
  }

private:
  void stop() {
    if (m_handle) {
      snd_pcm_close(m_handle);
      m_handle = 0;
    }
  }

  snd_pcm_t *m_handle;
};

class HwParams {
public:
  HwParams() :
    m_params(0) {

  }

  bool init(Device *dev) {
    int err;

    if (m_params) {
      return true;
    }

    err = snd_pcm_hw_params_malloc(&m_params);
    if (err < 0) {
      qWarning() << "Cannot allocate hardware parameters:" << snd_strerror(err);
      return false;
    }

    err = snd_pcm_hw_params_any(dev->handle(), m_params);
    if (err < 0) {
      qWarning() << "Cannot initialize hardware parameters:" << snd_strerror(err);
      return false;
    }

    err = snd_pcm_hw_params_set_access(dev->handle(), m_params,
				       SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
      qWarning() << "Cannot set hardware access:" << snd_strerror(err);
      return false;
    }

    err = snd_pcm_hw_params_set_format(dev->handle(), m_params, SND_PCM_FORMAT_S16);
    if (err < 0) {
      qWarning() << "Cannot set audio format:" << snd_strerror(err);
      return false;
    }

    err = snd_pcm_hw_params_set_rate(dev->handle(), m_params, 22050, 0);
    if (err < 0) {
      qWarning() << "Cannot set sample rate:" << snd_strerror(err);
      return false;
    }

    err = snd_pcm_hw_params_set_channels(dev->handle(), m_params, 1);
    if (err < 0) {
      qWarning() << "Cannot set audio channels:" << snd_strerror(err);
      return false;
    }

    err = snd_pcm_hw_params(dev->handle(), m_params);
    if (err < 0) {
      qWarning() << "Cannot set audio parameters:" << snd_strerror(err);
      return false;
    }

    return true;
  }

  ~HwParams() {
    snd_pcm_hw_params_free(m_params);
  }

private:
  snd_pcm_hw_params_t *m_params;
};

Alsa::Alsa(AudioOutput *parent) :
  AudioOutputInterface(parent),
  m_running(false),
  m_audio(parent),
  m_device(0) {

}

Alsa::~Alsa() {
  stop();

  delete m_device;
  m_device = 0;
}

void Alsa::start() {
  if (m_running) {
    return;
  }

  m_running = true;
  try {
    m_thread = std::thread(__run, this);
  } catch (...) {
    m_running = false;
    emit error();
  }
}

void Alsa::__run(Alsa *that) {
  that->run();
}

void Alsa::run() {
  while (m_running) {
    AudioBuffer buffer = m_audio->buffer();

    if (buffer.media.isEos()) {
      QMetaObject::invokeMethod(this, "drainAndFinish", Qt::QueuedConnection);
      return;
    }
    else if (buffer.media.isError()) {
      QMetaObject::invokeMethod(this, "drainAndError", Qt::QueuedConnection);
      return;
    }
    else if (!m_running) {
      QMetaObject::invokeMethod(this, "drainAndFinish", Qt::QueuedConnection);
    }


    while (m_running &&
	   buffer.data.size()) {
      int err = snd_pcm_wait(m_device->handle(), 100);
      qWarning() << err;
      snd_pcm_sframes_t frames = snd_pcm_avail(m_device->handle());
      ssize_t size =
	qMin((ssize_t)buffer.data.size(), snd_pcm_frames_to_bytes(m_device->handle(), frames));
      frames = snd_pcm_bytes_to_frames(m_device->handle(), size);
      int ret = snd_pcm_writei(m_device->handle(), buffer.data.constData(), frames);
      buffer.data.remove(0, size);
      if (ret != frames) {
	emit error();
	return;
      }
    }
  }
}

void Alsa::stop() {
  m_running = false;
  try {
    m_thread.join();
  } catch (...) {}
}

bool Alsa::connect() {
  if (m_device) {
    return true;
  }

  m_device = new Device;
  if (!m_device->open()) {
    delete m_device;
    m_device = 0;
    return false;
  }

  HwParams hwp;
  if (!hwp.init(m_device)) {
    delete m_device;
    m_device = 0;
    return false;
  }

  if (!m_device->init()) {
    delete m_device;
    m_device = 0;
    return false;
  }

  return true;
}

bool Alsa::isRunning() {
  return m_running;
}

void Alsa::drainAndFinish() {
  drain();

  emit finished();
}

void Alsa::drainAndError() {
  drain();

  emit error();
}

void Alsa::drain() {
  snd_pcm_drain(m_device->handle());
}
