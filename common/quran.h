/*
 * Copyright (c) 2020 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef QURAN_H
#define QURAN_H

#include <QObject>

class QDataStream;

class Quran : public QObject {
  Q_OBJECT
  Q_ENUMS(PlaybackState);

public:
  typedef enum {
      Stopped,
      Paused,
      Playing,
  } PlaybackState;
};
Q_DECLARE_METATYPE(Quran::PlaybackState);

QDataStream& operator<<(QDataStream& out, const Quran::PlaybackState& s);
QDataStream& operator>>(QDataStream& in, Quran::PlaybackState& s);

#endif /* QURAN_H */
