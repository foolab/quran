/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef OBOE_H
#define OBOE_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include "audiooutput.h"

namespace oboe {
  class AudioStream;
};

class Oboe : public AudioOutputInterface {
  Q_OBJECT

public:
  Oboe(AudioOutput *parent = 0);
  ~Oboe();

  bool connect();
  void stop();

  void start();

  bool isRunning();

private slots:
  void feedData();
  void drainAndFinish();
  void drainAndError();

private:
  void drain();

  AudioOutput *m_audio;
  oboe::AudioStream *m_stream;
  QByteArray m_buffer;
  QTimer m_timer;
};

#endif /* SLES_H */
