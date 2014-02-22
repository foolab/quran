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

#ifndef PULSE_H
#define PULSE_H

#include <QObject>
#include <QMutex>
#include <pulse/pulseaudio.h>
#include "audiooutput.h"

class AudioOutput;

class Pulse : public QObject {
  Q_OBJECT

public:
  Pulse(AudioOutput *parent = 0);
  ~Pulse();

  bool connect();
  void stop();

  void start();

signals:
  void error();
  void finished();
  void positionChanged(int index);

private slots:
  void drainAndFinish();

private:
  static void contextStateCallback(pa_context *ctx, Pulse *pulse);
  static void streamStateCallback(pa_stream *stream, Pulse *pulse);
  static void streamWriteCallback(pa_stream *stream, size_t length, Pulse *pulse);
  static void successCallback(pa_stream *stream, int success, Pulse *pulse);

  bool createStream();

  void writeData();

  AudioOutput *m_audio;
  pa_threaded_mainloop *m_loop;
  pa_context *m_ctx;
  pa_stream *m_stream;
  bool m_stop;
  bool m_started;
};

#endif /* PULSE_H */
