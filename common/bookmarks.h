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

#ifndef BOOKMARKS_H
#define BOOKMARKS_H

#include <QObject>
#include <QVariant>

class Settings;

class Bookmarks : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged);
  Q_PROPERTY(Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged);

public:
  Bookmarks(QObject *parent = 0);
  ~Bookmarks();

  Settings *settings();
  void setSettings(Settings *settings);

  Q_INVOKABLE static uint serialize(int sura, int aya);

  bool isBookmarked(uint bookmark) const;

  QList<uint> bookmarks() const;

  Q_INVOKABLE int sura(uint bookmark);
  Q_INVOKABLE int aya(uint bookmark);

  bool isEmpty();

public slots:
  void add(uint bookmark);
  void remove(uint bookmark);
  void add(int sura, int aya);
  void remove(int sura, int aya);
  void clear();

signals:
  void bookmarkAdded(uint bookmark);
  void bookmarkRemoved(uint bookmark);
  void cleared();
  void emptyChanged();
  void settingsChanged();

private:
  void deserialize(uint bookmark, int& sura, int& aya);

  QList<uint> m_bookmarks;

  Settings *m_settings;
};

#endif /* BOOKMARKS_H */
