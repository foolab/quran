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

#ifndef NUMBER_FORMATTER_H
#define NUMBER_FORMATTER_H

#include <QObject>

class NumberFormatter : public QObject {
  Q_OBJECT
  Q_PROPERTY(int format READ format WRITE setFormat NOTIFY formatChanged);

public:
  NumberFormatter(QObject *parent = 0);
  ~NumberFormatter();

  Q_INVOKABLE QString number(int number);

  Q_INVOKABLE QString toHindi(int number);

  int format() const;
  void setFormat(int format);

signals:
  void formatChanged();

private:
  int m_format;
};

#endif /* NUMBER_FORMATTER_H */
