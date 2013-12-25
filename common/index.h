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

#ifndef INDEX_H
#define INDEX_H

#include <QList>
#include <QPair>
#include <QString>
#include <QMap>
#include <QVariant>

class Index {
public:
  static bool write(const QString& path, const QList<QPair<off_t, size_t> >& offsets,
		    const QMap<QString, QVariant>& meta);

  static bool read(const QString& path, QList<QPair<off_t, size_t> >& offsets,
		   QMap<QString, QVariant>& meta);
};

#endif /* INDEX_H */
