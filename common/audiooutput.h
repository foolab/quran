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
#include <QMutex>
#include <QWaitCondition>
#include "media.h"

#ifndef ANDROID
class Pulse;
#else
class Sles;
#endif

class AudioBuffer {
public:
 AudioBuffer(const Media& m) : media(m) {}

  QByteArray data;
  Media media;
};

class AudioOutput : public QObject {
  Q_OBJECT

public:
  AudioOutput(QObject *parent = 0);
  ~AudioOutput();

  bool start();
  void stop();

  void play(const QList<AudioBuffer>& buffers);
  void play(const AudioBuffer& buffer);

  AudioBuffer buffer();

signals:
  void finished();
  void error();
  void positionChanged(int index);

private slots:
  void pulsePositionChanged(int index);

private:
  QMutex m_mutex;
  QWaitCondition m_cond;
  QList<AudioBuffer> m_buffers;

#ifndef ANDROID
  Pulse *m_out;
#else
  Sles *m_out;
#endif
};

#endif /* AUDIO_OUTPUT_H */
