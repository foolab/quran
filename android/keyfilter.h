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

#ifndef KEY_FILTER_H
#define KEY_FILTER_H

#include <QObject>

class KeyFilter : public QObject {
  Q_OBJECT

public:
  KeyFilter(QObject *parent = 0);
  ~KeyFilter();

protected:
  bool eventFilter(QObject *watched, QEvent *event);

signals:
  void backTriggered();
};

#endif /* KEY_FILTER_H */
