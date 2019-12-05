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

#ifndef AUDIO_OUTPUT_H
#define AUDIO_OUTPUT_H

#include <QObject>
#include <QTimer>
#include "media.h"

class AudioBuffer {
public:
  AudioBuffer(const Media& m) : media(m) {}

  QByteArray data;
  Media media;
};

class AudioOutput : public QObject {
  Q_OBJECT

public:
  static AudioOutput *create(QObject *parent);

  virtual ~AudioOutput();

  virtual bool start();
  virtual void stop();

  virtual void pause();
  virtual void resume();

  int numberOfBuffers();

  bool isRunning();
  bool isPaused();

  void addBuffer(const AudioBuffer& buffer);

protected:
  AudioOutput(QObject *parent = 0);

  virtual bool connect() = 0;
  virtual bool writeData(QByteArray& data) = 0;
  virtual bool hasFrames() = 0;

signals:
  void finished();
  void error();
  void positionChanged(int index);

private slots:
  void timeout();

private:
  bool m_paused;
  QList<AudioBuffer> m_buffers;
  QTimer m_timer;
  QByteArray m_data;
};

#endif /* AUDIO_OUTPUT_H */
