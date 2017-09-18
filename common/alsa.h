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

#ifndef ALSA_H
#define ALSA_H

#include <QObject>
#include <thread>
#include "audiooutput.h"

class Device;

class Alsa : public AudioOutputInterface {
  Q_OBJECT

public:
  Alsa(AudioOutput *parent = 0);
  ~Alsa();

  bool connect();
  void stop();

  void start();

  bool isRunning();

private slots:
  void drainAndFinish();
  void drainAndError();

private:
  static void __run(Alsa *that);

  void run();
  void drain();

  bool m_running;
  std::thread m_thread;
  AudioOutput *m_audio;
  Device *m_device;
};

#endif /* ALSA_H */
