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

#ifndef MEDIA_CODEC_H
#define MEDIA_CODEC_H

#include <QObject>
#include <QTimer>
#include <QList>
#include "media.h"
#include "audiooutput.h"

class MediaResampler;
struct AVFormatContext;
struct AVPacket;
struct AVCodecContext;

class MediaCodec : public QObject {
  Q_OBJECT

public:
  MediaCodec(Media media, QObject *parent = 0);
  ~MediaCodec();

  bool isDone();
  void stop();

  QList<AudioBuffer> buffers();

signals:
  void buffersAvailable();

private slots:
  void decodePacket();

private:
  void flushDecoder(AudioBuffer& buffer);
  void cleanup();
  bool decode(AVPacket *pkt, AudioBuffer& buffer);
  AVFormatContext *context(const QByteArray& data);

  Media m_media;
  QTimer m_timer;
  QList<AudioBuffer> m_buffers;

  MediaResampler *m_resampler;
  AVFormatContext *m_ctx;
  AVCodecContext *m_codec;
};

#endif /* MEDIA_CODEC_H */
