/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#include "oboe.h"
#include <oboe/Oboe.h>
#include <QDebug>

#define TIMER_INTERVAL_MS      25
#define BUFFER_CAPACITY        2000  // Arbitrary

Oboe::Oboe(AudioOutput *parent) :
  AudioOutputInterface(parent),
  m_audio(parent),
  m_stream(0) {

  m_timer.setSingleShot(false);
  m_timer.setInterval(TIMER_INTERVAL_MS);

  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(feedData()));
}

Oboe::~Oboe() {
  stop();

  delete m_stream;
  m_stream = 0;
}

void Oboe::start() {
  if (isRunning()) {
    return;
  }

  m_stream->requestStart();

  m_timer.start();
}

void Oboe::stop() {
  m_timer.stop();
  m_stream->requestStop();
  m_buffer.clear();
}

bool Oboe::connect() {
  if (m_stream) {
    return true;
  }

  oboe::AudioStreamBuilder builder;
  builder.setDirection(oboe::Direction::Output);
  builder.setPerformanceMode(oboe::PerformanceMode::PowerSaving);
  builder.setSharingMode(oboe::SharingMode::Exclusive);
  builder.setFormat(oboe::AudioFormat::I16);
  builder.setChannelCount(oboe::ChannelCount::Mono);
  builder.setSampleRate(22050);
  builder.setUsage(oboe::Usage::Media);
  builder.setContentType(oboe::Speech);
  builder.setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Medium);
  builder.setBufferCapacityInFrames(BUFFER_CAPACITY);

  oboe::Result result = builder.openStream(&m_stream);
  if (result != oboe::Result::OK) {
    qWarning() << "Failed to open managed stream" << oboe::convertToText(result);
    delete m_stream;
    m_stream = 0;
    return false;
  }

  if (m_stream->getFormat() != oboe::AudioFormat::I16 ||
      m_stream->getChannelCount() != oboe::ChannelCount::Mono ||
      m_stream->getSampleRate() != 22050) {
    qWarning() << "Failed to set audio stream properties";
    delete m_stream;
    m_stream = 0;
    return false;
  }

  return true;
}

bool Oboe::isRunning() {
  return m_stream && m_stream->getState() == oboe::StreamState::Started && m_timer.isActive();
}

void Oboe::feedData() {
  if (m_buffer.isEmpty()) {
    AudioBuffer buffer = m_audio->buffer();

    if (buffer.media.isEos()) {
      QMetaObject::invokeMethod(this, "drainAndFinish", Qt::QueuedConnection);
      return;
    } else if (buffer.media.isError()) {
      QMetaObject::invokeMethod(this, "drainAndError", Qt::QueuedConnection);
      return;
    }

    emit positionChanged(buffer.media.index());

    m_buffer = buffer.data;
  }

  int32_t frames = m_buffer.size() / m_stream->getBytesPerFrame();
  oboe::ResultWithValue<int32_t> r = m_stream->write(m_buffer.constData(), frames, 0);
  if (!r) {
    qWarning() << "Error writing to stream" << oboe::convertToText(r.error());
    emit error();
    return;
  }

  m_buffer.remove(0, r.value() * m_stream->getBytesPerFrame());
}

void Oboe::drain() {
  // TODO: This is not correct
  m_stream->requestFlush();
}

void Oboe::drainAndFinish() {
  drain();

  emit finished();
}

void Oboe::drainAndError() {
  drain();

  emit error();
}
