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
#include <QQmlInfo>

#define BUFFER_CAPACITY        2000  // Arbitrary

Oboe::Oboe(QObject *parent) :
  m_stream(0) {

}

Oboe::~Oboe() {
  stop();

  delete m_stream;
  m_stream = 0;
}

bool Oboe::start() {
  oboe::Result r = m_stream->requestStart();
  if (r != oboe::Result::OK) {
    qmlInfo(this) << "Failed to start audio stream" << oboe::convertToText(r);
    return false;
  }

  return AudioOutput::start();
}

void Oboe::stop() {
  m_stream->requestStop();
  return AudioOutput::stop();
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
    qmlInfo(this) << "Failed to open audio stream" << oboe::convertToText(result);
    delete m_stream;
    m_stream = 0;
    return false;
  }

  if (m_stream->getFormat() != oboe::AudioFormat::I16 ||
      m_stream->getChannelCount() != oboe::ChannelCount::Mono ||
      m_stream->getSampleRate() != 22050) {
    qmlInfo(this) << "Failed to set audio stream properties";
    delete m_stream;
    m_stream = 0;
    return false;
  }

  return true;
}

bool Oboe::writeData(QByteArray& data) {
  int32_t frames = data.size() / m_stream->getBytesPerFrame();
  oboe::ResultWithValue<int32_t> r = m_stream->write(data.constData(), frames, 0);
  if (!r) {
    qmlInfo(this) << "Error writing to stream" << oboe::convertToText(r.error());
    return false;
  }

  data.remove(0, r.value() * m_stream->getBytesPerFrame());

  return true;
}

bool Oboe::hasFrames() {
  oboe::ResultWithValue<int32_t> r = m_stream->getAvailableFrames();

  if (!r) {
    qmlInfo(this) << "Error getting stream available frames" << oboe::convertToText(r.error());
    return false;
  }

  return r.value() > 0;
}
