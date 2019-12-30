/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef TEXT_PROVIDER_H
#define TEXT_PROVIDER_H

#include <QList>
#include <QString>

class TextProviderPrivate;

class TextProvider {
public:
  TextProvider(int id, const QString& uuid, const QString& dataFile, const QString& indexFile);
  ~TextProvider();

  bool load();

  int id() const;

  QString uuid() const;

  QString text(int verse, off_t start) const;

private:
  void unload();

  TextProviderPrivate *d_ptr;
};

#endif /* TEXT_PROVIDER_H */
