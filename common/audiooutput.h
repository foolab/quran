/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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

class AudioOutputInterface : public QObject {
  Q_OBJECT

public:
  virtual ~AudioOutputInterface() {}
  virtual bool isRunning() = 0;
  virtual bool connect() = 0;
  virtual void start() = 0;
  virtual void stop() = 0;

protected:
  AudioOutputInterface(QObject *parent = 0) :
    QObject(parent) {}

signals:
  void error();
  void finished();
  void positionChanged(int index);
};

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

  void play(const AudioBuffer& buffer);

  AudioBuffer buffer();

signals:
  void finished();
  void error();
  void positionChanged(int index);

private slots:
  void audioPositionChanged(int index);

private:
  QMutex m_mutex;
  QWaitCondition m_cond;
  QList<AudioBuffer> m_buffers;

  AudioOutputInterface *m_out;
};

#endif /* AUDIO_OUTPUT_H */
