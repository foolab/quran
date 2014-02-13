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

#ifndef AUDIO_OUTPUT_H
#define AUDIO_OUTPUT_H

#include <QObject>

class Media;
class Pulse;

class AudioBuffer {
public:
 AudioBuffer() : rate(0), channels(0), media(0) {}
  int rate;
  unsigned short channels;
  QByteArray data;
  const Media *media;
};

class AudioOutput : public QObject {
  Q_OBJECT

public:
  AudioOutput(QObject *parent = 0);
  ~AudioOutput();

  void start();
  void stop();

  void queue(AudioBuffer buffer);

signals:
  void finished();
  void error();
  void positionChanged(int index);

private:
  QList<AudioBuffer> m_buffers;

  const Media *m_media;
  Pulse *m_pulse;
};

#endif /* AUDIO_OUTPUT_H */
