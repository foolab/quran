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

#include "sles.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class Engine {
public:
  Engine() :
    m_engine(0),
    m_object(0) {

    if (slCreateEngine(&m_object, 0, NULL, 0, NULL, NULL) != SL_RESULT_SUCCESS) {
      return;
    }

    if ((*m_object)->Realize(m_object, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
      return;
    }

    if ((*m_object)->GetInterface(m_object, SL_IID_ENGINE, &m_engine) != SL_RESULT_SUCCESS) {
      return;
    }
  }

  ~Engine() {
    if (m_object) {
      (*m_object)->Destroy(m_object);
      m_object = 0;
    }
  }

  bool isOk() {
    return m_engine != 0 && m_object != 0;
  }

  SLEngineItf m_engine;
  SLObjectItf m_object;
};

class Mix {
public:
  Mix(SLEngineItf engine) :
  m_engine(engine),
  m_object(0) {
    const SLInterfaceID ids[] = {SL_IID_VOLUME};
    const SLboolean req[] = {SL_BOOLEAN_FALSE};

    if ((*m_engine)->CreateOutputMix(m_engine, &m_object, 1, ids, req) != SL_RESULT_SUCCESS) {
      return;
    }

    if ((*m_object)->Realize(m_object, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
      return;
    }
  }

  ~Mix() {
    if (m_object) {
      (*m_object)->Destroy(m_object);
      m_object = 0;
    }
  }

  bool isOk() {
    return m_object != 0;
  }

public:
  SLEngineItf m_engine;
  SLObjectItf m_object;
};

class Sink {
public:
  Sink(SLEngineItf engine, SLObjectItf mix) :
    m_engine(engine),
    m_mix(mix),
    m_playerObject(0),
    m_player(0),
    m_queue(0) {

    /* source */
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq =
      {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_22_05, /* 22050 */
				   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
				   SL_SPEAKER_FRONT_CENTER, /* 1 channel */
				   SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    /* sink */
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, m_mix};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    /* player */
    const SLInterfaceID ids1[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req1[] = {SL_BOOLEAN_TRUE};
    if ((*m_engine)->CreateAudioPlayer(m_engine, &m_playerObject, &audioSrc, &audioSnk,
				       1, ids1, req1) != SL_RESULT_SUCCESS) {
      return;
    }

    if ((*m_playerObject)->Realize(m_playerObject, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
      return;
    }

    /* player interface */
    if ((*m_playerObject)->GetInterface(m_playerObject,
					SL_IID_PLAY, &m_player) != SL_RESULT_SUCCESS) {
      return;
    }

    /* buffer queue interface */
    if ((*m_playerObject)->GetInterface(m_playerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
					&m_queue) != SL_RESULT_SUCCESS) {
      return;
    }
  }

  ~Sink() {
    if (m_playerObject) {
      (*m_playerObject)->Destroy(m_playerObject);
      m_playerObject = 0;
    }

    m_player = 0;
    m_queue = 0;
  }

  bool isOk() {
    return m_playerObject != 0 && m_player != 0 && m_queue != 0;
  }

  bool play(QByteArray& data) {
    m_data = data;

    if ((*m_queue)->Enqueue(m_queue, m_data.constData(), m_data.size()) != SL_RESULT_SUCCESS) {
      return false;
    }

    return true;
  }

  void stop() {
    if (m_player) {
      (*m_player)->SetPlayState(m_player,
				SL_PLAYSTATE_STOPPED);
    }

    if (m_queue) {
      (*m_queue)->Clear(m_queue);
    }

    m_data.clear();
  }

  SLEngineItf m_engine;
  SLObjectItf m_mix;
  SLObjectItf m_playerObject;
  SLPlayItf m_player;
  SLAndroidSimpleBufferQueueItf m_queue;
  QByteArray m_data;
};

void Sles::slesCallback(SLAndroidSimpleBufferQueueItf q, void *context) {
  Q_UNUSED(q);

  Sles *sles = (Sles *)context;

  sles->writeData();
}

Sles::Sles(AudioOutput *parent) :
  QObject(parent),
  m_audio(parent),
  m_stop(false),
  m_started(false),
  m_engine(0),
  m_mix(0),
  m_sink(0) {

}

Sles::~Sles() {
  stop();

  delete m_sink;
  m_sink = 0;

  delete m_mix;
  m_mix = 0;

  delete m_engine;
  m_engine = 0;
}

void Sles::start() {
  if (!m_started) {
    /* callback */
    if ((*m_sink->m_queue)->RegisterCallback(m_sink->m_queue,
					     slesCallback, this) != SL_RESULT_SUCCESS) {
      emit error();
      return;
    }

    if ((*m_sink->m_player)->SetPlayState(m_sink->m_player,
					  SL_PLAYSTATE_PLAYING) != SL_RESULT_SUCCESS) {
      emit error();
      return;
    }

    writeData();

    m_started = true;
  }
}

void Sles::stop() {
  if (m_sink) {
    m_sink->stop();
  }
}

bool Sles::connect() {
  if (m_engine) {
    return true;
  }

  m_engine = new Engine;

  if (!m_engine->isOk()) {
    delete m_engine;
    m_engine = 0;
    return false;
  }

  m_mix = new Mix(m_engine->m_engine);
  if (!m_mix->isOk()) {
    delete m_mix;
    m_mix = 0;

    delete m_engine;
    m_engine = 0;

    return false;
  }

  m_sink = new Sink(m_engine->m_engine, m_mix->m_object);
  if (!m_sink->isOk()) {
    delete m_sink;
    m_sink = 0;

    delete m_mix;
    m_mix = 0;

    delete m_engine;
    m_engine = 0;

    return false;
  }

  return true;
}

bool Sles::isRunning() {
  return m_started;
}

void Sles::writeData() {
  // Can be called from the GUI thread or a pulseaudio thread.
  if (m_stop) {
    return;
  }

  AudioBuffer buffer = m_audio->buffer();

  if (buffer.media.isEos()) {
    QMetaObject::invokeMethod(this, "drainAndFinish", Qt::QueuedConnection);
    m_stop = true;
    return;
  }
  else if (buffer.media.isError()) {
    QMetaObject::invokeMethod(this, "drainAndError", Qt::QueuedConnection);
    m_stop = true;
    return;
  }
  else if (m_stop) {
    QMetaObject::invokeMethod(this, "drainAndFinish", Qt::QueuedConnection);
    return;
  }

  emit positionChanged(buffer.media.index());

  if (!m_sink->play(buffer.data)) {
    emit error();
  }
}

void Sles::drain() {
  // Android does not need that because OpenSL ES callback gets called when
  // we are done playing the data (Which is a drain), we reach writeData()
  // which detects EOS and sets m_stop. It will then never try to playback
  // any buffers because of that flag.
  // The callback will not be called too because we did not write any data
  // so everything should stop.
}

void Sles::drainAndFinish() {
  drain();

  emit finished();
}

void Sles::drainAndError() {
  drain();

  emit error();
}
