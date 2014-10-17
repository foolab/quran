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

#include "sles.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

Sles::Sles(AudioOutput *parent) :
  QObject(parent) {

}

Sles::~Sles() {

}

void Sles::start() {

}

void Sles::stop() {

}

bool Sles::connect() {
  //  slCreateEngine(
}

bool Sles::isRunning() {

}
#if 0
signals:
  void error();
  void finished();
  void positionChanged(int index);

private:
  AudioOutput *m_audio;
  bool m_stop;
  bool m_started;
};

#endif
