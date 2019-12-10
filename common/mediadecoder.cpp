/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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

#include "mediadecoder.h"
#include "media.h"
#include "mediaplaylist.h"
#include <QDebug>
#include "audiooutput.h"
#include "mediacodec.h"
#include "recitationdataprovider.h"

#define TIMER_INTERVAL_MS            500
#define NUM_DECODED_BUFFERS          10 // Arbitrary

// This class does not have an error signal. It will queue an error AudioBuffer
// in case of an error. This is to enable playing audio for as long as possible.
MediaDecoder::MediaDecoder(RecitationDataProvider *provider, QObject *parent) :
  QObject(parent),
  m_provider(provider),
  m_codec(0),
  m_audio(0) {

  m_timer.setSingleShot(false);
  m_timer.setInterval(TIMER_INTERVAL_MS);
  m_timer.setTimerType(Qt::VeryCoarseTimer);

  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(decodeMedia()));
}

MediaDecoder::~MediaDecoder() {

}

void MediaDecoder::decodeMedia() {
  if (m_media.isEmpty()) {
    return;
  }

  if ((m_audio && m_audio->numberOfBuffers() >= NUM_DECODED_BUFFERS) ||
      (!m_audio && m_buffers.size() >= NUM_DECODED_BUFFERS)) {
    return;
  }

  if (m_codec) {
    if (!m_codec->isDone()) {
      return;
    } else {
      buffersAvailable();
      delete m_codec;
      m_codec = 0;
    }
  }

  Media media = m_media.takeFirst();
  if (media.isEos()) {
    // We are done.
    play(AudioBuffer(Media::eos()));
    return;
  } else if (media.isError()) {
    play(AudioBuffer(Media::error()));
    return;
  }

  QByteArray data = m_provider->data(media);

  if (data.isEmpty()) {
    play(AudioBuffer(Media::error()));
    return;
  }

  m_codec = new MediaCodec(media, data);
  QObject::connect(m_codec, SIGNAL(buffersAvailable()), this, SLOT(buffersAvailable()));
}

void MediaDecoder::buffersAvailable() {
  if (m_codec) {
    for (const AudioBuffer b : m_codec->buffers()) {
      play(b);
    }
  }
}

void MediaDecoder::start() {
  m_timer.start();
}

void MediaDecoder::stop() {
  m_timer.stop();
  m_media.clear();
  m_buffers.clear();

  if (m_codec) {
    m_codec->stop();
    delete m_codec;
    m_codec = 0;
  }

  if (m_audio) {
    play(AudioBuffer(Media::eos()));
  }
}

void MediaDecoder::addMedia(const Media& media) {
  m_media << media;
}

void MediaDecoder::setOutput(AudioOutput *audio) {
  m_audio = audio;

  foreach (const AudioBuffer& buffer, m_buffers) {
    m_audio->addBuffer(buffer);
  }

  m_buffers.clear();
}

void MediaDecoder::play(const AudioBuffer& buffer) {
  if (m_audio) {
    m_audio->addBuffer(buffer);
  } else {
    m_buffers << buffer;
  }
}
