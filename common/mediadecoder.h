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

#ifndef MEDIA_DECODER_H
#define MEDIA_DECODER_H

#include <QObject>
#include <QTimer>

class Media;
class AudioOutput;
class AudioBuffer;
class MediaCodec;
class RecitationDataProvider;

class MediaDecoder : public QObject {
  Q_OBJECT

public:
  MediaDecoder(RecitationDataProvider *provider, QObject *parent = 0);
  ~MediaDecoder();

  void start();
  void stop();

  void addMedia(const Media& media);

  void setOutput(AudioOutput *audio);

private slots:
  void decodeMedia();
  void buffersAvailable();

private:
  void play(const AudioBuffer& buffer);

  RecitationDataProvider *m_provider;
  MediaCodec *m_codec;
  AudioOutput *m_audio;
  QList<AudioBuffer> m_buffers;

  QTimer m_timer;
  QList<Media> m_media;
};

#endif /* MEDIA_DECODER_H */
