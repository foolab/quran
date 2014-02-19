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

#ifndef MEDIA_DECODER_H
#define MEDIA_DECODER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

extern "C" {
#include <libavformat/avformat.h>
};

class Media;
class AudioOutput;
class AudioBuffer;
class MediaResampler;

class MediaDecoder : public QThread {
  Q_OBJECT

public:
  MediaDecoder(QObject *parent = 0);
  ~MediaDecoder();

  void stop();

  void addMedia(Media *media);

  void setOutput(AudioOutput *audio);

protected:
  void run();

private:
  void cleanup(AVFormatContext *ctx);
  bool decode(AVFormatContext *ctx, const Media *media);
  bool decode(AVCodecContext *ctx, AVPacket *pkt, AudioBuffer& buffer, MediaResampler *resampler);
  AVFormatContext *context(const QByteArray& data);

  bool stopRequested();
  Media *media();
  void play(const AudioBuffer& buffer);

  AudioOutput *m_audio;
  QMutex m_audioMutex;
  QList<AudioBuffer> m_buffers;

  bool m_stop;
  QMutex m_mutex;
  QWaitCondition m_cond;
  QList<Media *> m_media;
};

#endif /* MEDIA_DECODER_H */
