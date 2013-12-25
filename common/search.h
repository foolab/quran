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

#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QVariantList>

class SearchPrivate;

class Search : public QObject {
  Q_OBJECT

public:
  Search(const QString& path, QObject *parent = 0);
  ~Search();

  Q_INVOKABLE QVariantList search(const QString& query, bool matchWholeWords);

  Q_INVOKABLE bool enable();

public slots:
  void disable();

private:
  SearchPrivate *d_ptr;
};

#endif /* SEARCH_H */
