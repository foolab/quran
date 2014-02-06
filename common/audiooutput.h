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
#include <pulse/simple.h>

class AudioPolicy;

class AudioBuffer {
public:
  unsigned long rate;
  unsigned short channels;
  QByteArray data;
  int chapter;
  int verse;
  int index;
};

class AudioOutput : public QObject {
  Q_OBJECT

public:
  AudioOutput(QObject *parent = 0);
  ~AudioOutput();

  void finish();

public slots:
  void play(AudioBuffer *buffer);

signals:
  void finished();
  void error();
  void positionChanged(int chapter, int verse, int index);

private slots:
  void process();
  void policyAcquired();
  void policyLost();
  void policyDenied();
  void playNext();
  void releasePolicy();

private:
  bool finishRequested();

  AudioPolicy *m_policy;
  QMutex m_mutex;
  bool m_finish;
  bool m_acquired;
  QList<AudioBuffer *> m_buffers;

  pa_sample_spec m_spec;
  pa_simple *m_simple;

  int m_index;
};

#endif /* AUDIO_OUTPUT_H */
