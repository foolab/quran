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

#include "sles.h"
#include <SLES/OpenSLES.h>
#include <QMutex>
#include <QDebug>

#define BUFFER_SIZE 4096
#define NUM_BUFFERS 2

class Engine {
public:
  Engine() :
    m_engine(0),
    m_object(0) {

    SLresult result;

    result = slCreateEngine(&m_object, 0, NULL, 0, NULL, NULL);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to create OpenSL ES engine" << result;
      return;
    }

    result = (*m_object)->Realize(m_object, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to realize OpenSL ES engine" << result;
      return;
    }

    result = (*m_object)->GetInterface(m_object, SL_IID_ENGINE, &m_engine);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to get OpenSL ES engine interface" << result;
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

    SLresult result;
    result = (*m_engine)->CreateOutputMix(m_engine, &m_object, 1, ids, req);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to create OpenSL ES output mix" << result;
      return;
    }

    result = (*m_object)->Realize(m_object, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to realize OpenSL ES output mix" << result;
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
    SLDataLocator_BufferQueue loc_bufq =
      {SL_DATALOCATOR_BUFFERQUEUE, NUM_BUFFERS};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_22_05, /* 22050 */
				   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
				   SL_SPEAKER_FRONT_CENTER, /* 1 channel */
				   SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    /* sink */
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, m_mix};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    /* player */
    const SLInterfaceID ids1[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req1[] = {SL_BOOLEAN_TRUE};

    SLresult result;
    result = (*m_engine)->CreateAudioPlayer(m_engine, &m_playerObject, &audioSrc, &audioSnk,
					    1, ids1, req1);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to create OpenSL ES audio player" << result;
      return;
    }

    result = (*m_playerObject)->Realize(m_playerObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to realize OpenSL ES audio player" << result;
      return;
    }

    /* player interface */
    result = (*m_playerObject)->GetInterface(m_playerObject,
					     SL_IID_PLAY, &m_player);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to get OpenSL ES audio player interface" << result;
      return;
    }

    /* buffer queue interface */
    result = (*m_playerObject)->GetInterface(m_playerObject, SL_IID_BUFFERQUEUE,
					     &m_queue);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to get OpenSL ES buffer queue interface" << result;
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
    int size = qMin(data.size(), BUFFER_SIZE);
    QByteArray d(data.constData(), size);
    {
      QMutexLocker l(&m_lock);
      if (m_data.size() == NUM_BUFFERS) {
	return true;
      }
      m_data << d;
    }

    SLresult result = (*m_queue)->Enqueue(m_queue, d.constData(), d.size());
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to enqueue OpenSL ES buffer" << result;
      return false;
    }

    data.remove(0, size);

    return true;
  }

  void pop() {
    QMutexLocker l(&m_lock);
    m_data.takeFirst();
  }

  static void slesCallback(SLBufferQueueItf q, void *context) {
    Q_UNUSED(q);
    Sink *that = (Sink *) context;
    that->pop();
  }

  bool registerCallback() {
    SLresult result = (*m_queue)->RegisterCallback(m_queue, slesCallback, this);
    if (result != SL_RESULT_SUCCESS) {
      qWarning() << "Failed to register OpenSL ES buffer queue callback" << result;
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

  QList<QByteArray> m_data;
  QMutex m_lock;
  SLEngineItf m_engine;
  SLObjectItf m_mix;
  SLObjectItf m_playerObject;
  SLPlayItf m_player;
  SLBufferQueueItf m_queue;
};

Sles::Sles(QObject *parent) :
  AudioOutput(parent),
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

bool Sles::start() {
  if (!m_sink->registerCallback()) {
    return false;
  }

  SLresult result = (*m_sink->m_player)->SetPlayState(m_sink->m_player,
						      SL_PLAYSTATE_PLAYING);
  if (result != SL_RESULT_SUCCESS) {
    qWarning() << "Failed to set OpenSL ES player state to playing" << result;
    return false;
  }

  return AudioOutput::start();
}

void Sles::stop() {
  if (m_sink) {
    m_sink->stop();
  }

  return AudioOutput::stop();
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

bool Sles::writeData(QByteArray& data) {
  return m_sink->play(data);
}

bool Sles::hasFrames() {
  QMutexLocker l(&m_sink->m_lock);
  return !m_sink->m_data.isEmpty();
}
