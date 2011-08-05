/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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

#ifndef PAGE_POSITION_CONTROLLER_H
#define PAGE_POSITION_CONTROLLER_H

#include <QObject>

class PagePositionController : public QObject {
  Q_OBJECT
  Q_PROPERTY(int sura READ sura WRITE setSura NOTIFY suraChanged);
  Q_PROPERTY(int aya READ aya WRITE setAya NOTIFY ayaChanged);

public:
  PagePositionController(QObject *parent = 0);
  ~PagePositionController();

  void setSura(int sura);
  int sura() const;

  void setAya(int aya);
  int aya() const;

  Q_INVOKABLE bool isValid();

public slots:
  void reset();
  void set(int sura, int aya);

signals:
  void suraChanged();
  void ayaChanged();

private:
  int m_sura;
  int m_aya;
};

#endif /* PAGE_POSITION_CONTROLLER_H */
