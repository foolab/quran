/*
 * Copyright (c) 2017-2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef AUDIO_POLICY_H
#define AUDIO_POLICY_H

#include <QObject>
#include <QAndroidJniObject>

class AudioPolicy : public QObject {
  Q_OBJECT

public:
  AudioPolicy(QObject *parent = 0);
  ~AudioPolicy();

  bool acquire();
  void release();

signals:
  void acquired();
  void lost();
  void denied();

private:
  QAndroidJniObject m_obj;
};

#endif /* AUDIO_POLICY_H */
