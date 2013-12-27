/*
 * Copyright (c) 2011-2013 Mohammed Sameer <msameer@foolab.org>.
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

#include "numberformatter.h"

NumberFormatter::NumberFormatter(QObject *parent) :
  QObject(parent), m_format(-1) {
}

NumberFormatter::~NumberFormatter() {

}


int NumberFormatter::format() const {
  return m_format;
}

void NumberFormatter::setFormat(int format) {
  if (NumberFormatter::format() != format) {
    m_format = format;
    emit formatChanged();
  }
}

QString NumberFormatter::number(int number) {
  if (m_format != 0) {
    return QString("%1").arg(number);
  }

  return toHindi(number);
}

QString NumberFormatter::toHindi(int number) {
  QString in = QString("%1").arg(number);

  QString result;
  QChar c;

  for (int x = 0; x < in.size(); x++) {
    c = in.at(x);
    if (c.isNumber() && c.unicode() < 0x0660) {
      result.append(c.unicode() + (0x0660 - 0x30));
    }
    else {
      result.append(c);
    }
  }

  return result;
}
