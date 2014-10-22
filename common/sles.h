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

#ifndef SLES_H
#define SLES_H

#include <QObject>
#include <QMutex>
#include "audiooutput.h"

class AudioOutput;
class Engine;
class Mix;
class Sink;
typedef const struct SLAndroidSimpleBufferQueueItf_ * const * SLAndroidSimpleBufferQueueItf;

class Sles : public QObject {
  Q_OBJECT

public:
  Sles(AudioOutput *parent = 0);
  ~Sles();

  bool connect();
  void stop();

  void start();

  bool isRunning();

signals:
  void error();
  void finished();
  void positionChanged(int index);

private slots:
  void drainAndFinish();
  void drainAndError();

private:
  static void slesCallback(SLAndroidSimpleBufferQueueItf q, void *context);
  void writeData();
  void drain();

  AudioOutput *m_audio;
  bool m_stop;
  bool m_started;

  Engine *m_engine;
  Mix *m_mix;
  Sink *m_sink;
};

#endif /* SLES_H */
