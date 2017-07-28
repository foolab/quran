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

#ifndef PART_INFO_H
#define PART_INFO_H

#include <QObject>

struct Part;

class PartInfo {
public:
  PartInfo();
  PartInfo(int index);
  virtual ~PartInfo();

  int index() const;
  int firstPage() const;
  int numberOfPages() const;
  QString name() const;

protected:
  void init(int index);
  Part *m_part;
};

class DeclarativePartInfo : public QObject, public PartInfo {
  Q_OBJECT
  Q_PROPERTY(int part READ part WRITE setPart NOTIFY partChanged);
  Q_PROPERTY(int firstPage READ firstPage NOTIFY partChanged);
  Q_PROPERTY(QString name READ name NOTIFY partChanged);

public:
  DeclarativePartInfo(QObject *parent = 0);

  int part() const;
  void setPart(int part);

signals:
  void partChanged();
};

#endif /* PART_INFO_H */
