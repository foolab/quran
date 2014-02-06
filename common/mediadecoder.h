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
#include <QMutex>

extern "C" {
#include <libavformat/avformat.h>
};

class MediaPlaylist;
class Media;
class AudioBuffer;

class MediaDecoder : public QObject {
  Q_OBJECT

public:
  MediaDecoder(MediaPlaylist *list, QObject *parent = 0);
  ~MediaDecoder();

  void finish();

signals:
  void finished();
  void error();
  void play(AudioBuffer *buffer);

private slots:
  void process();

private:
  void cleanup(AVFormatContext *ctx);
  bool finishRequested();
  bool decode(AVFormatContext *ctx, const Media *media);
  bool decode(AVCodecContext *ctx, AVPacket *pkt, const Media *media);
  AVFormatContext *context(const QByteArray& data);

  MediaPlaylist *m_list;
  QMutex m_mutex;
  bool m_finish;
};

#endif /* MEDIA_DECODER_H */
