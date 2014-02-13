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

#include <QObject>

extern "C" {
#include <libavformat/avformat.h>
};

class MediaPlaylist;
class Media;
class AudioOutput;
class AudioBuffer;

class MediaDecoder : public QObject {
  Q_OBJECT

public:
  MediaDecoder(MediaPlaylist *list, QObject *parent = 0);
  ~MediaDecoder();

  void start();
  void stop();

  void policyAcquired();

signals:
  void finished();
  void error();
  void audioFinished();
  void audioError();
  void positionChanged(int chapter, int verse);

private slots:
  void process();
  void audioPositionChanged(int index);

private:
  void cleanup(AVFormatContext *ctx);
  bool decode(AVFormatContext *ctx, const Media *media);
  bool decode(AVCodecContext *ctx, AVPacket *pkt, AudioBuffer& buffer);
  AVFormatContext *context(const QByteArray& data);

  MediaPlaylist *m_list;
  AudioOutput *m_audio;
  QList<Media *> m_media;
};

#endif /* MEDIA_DECODER_H */
