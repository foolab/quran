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

#ifndef BOOKMARKS_H
#define BOOKMARKS_H

#include <QObject>
#include <QVariant>

class Settings;

class Bookmarks : public QObject {
  Q_OBJECT

public:
  Bookmarks(Settings *settings, QObject *parent = 0);
  ~Bookmarks();

  uint serialize(int sura, int aya);

  Q_INVOKABLE bool isBookmarked(uint bookmark) const;

  Q_INVOKABLE QVariantList bookmarks() const;

  Q_INVOKABLE int sura(uint bookmark);
  Q_INVOKABLE int aya(uint bookmark);

public slots:
  void add(uint bookmark);
  void remove(uint bookmark);
  void removeByIndex(int index);
  void clear();

signals:
  void bookmarkAdded(uint bookmark, int index);
  void bookmarkRemoved(uint bookmark, int index);
  void cleared();

private:
  void deserialize(uint bookmark, int& sura, int& aya);

  QList<uint> m_bookmarks;

  Settings *m_settings;
};

#endif /* BOOKMARKS_H */
