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

#include "quran.h"
#include <QDataStream>

QDataStream& operator<<(QDataStream& out, const Quran::PlaybackState& s) {
  return out << static_cast<int>(s);
}

QDataStream& operator>>(QDataStream& in, Quran::PlaybackState& s) {
  int val;
  QDataStream& ret = in >> val;
  s = static_cast<Quran::PlaybackState>(val);
  return ret;
}
