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

#include "bookmarks.h"
#include "settings.h"

Bookmarks::Bookmarks(QObject *parent) :
  QObject(parent), m_settings(0) {

}

Bookmarks::~Bookmarks() {
  m_settings = 0;
}

Settings *Bookmarks::settings() {
  return m_settings;
}

void Bookmarks::setSettings(Settings *settings) {
  if (m_settings != settings) {
    m_settings = settings;

    m_bookmarks = m_settings->bookmarks();
    qSort(m_bookmarks);

    emit settingsChanged();
  }
}

bool Bookmarks::isEmpty() {
  return m_bookmarks.isEmpty();
}

uint Bookmarks::serialize(int sura, int aya) {
  uint bookmark = sura;

  bookmark <<= 16;
  bookmark |= aya;

  return bookmark;
}

bool Bookmarks::isBookmarked(uint bookmark) const {
  return m_bookmarks.indexOf(bookmark) != -1;
}

void Bookmarks::add(uint bookmark) {
  if (m_bookmarks.indexOf(bookmark) == -1) {
    m_bookmarks << bookmark;
    qSort(m_bookmarks);
    m_settings->setBookmarks(m_bookmarks);

    emit bookmarkAdded(bookmark);

    if (m_bookmarks.size() == 1) {
      emit emptyChanged();
    }
  }
}

void Bookmarks::remove(uint bookmark) {
  int index = m_bookmarks.indexOf(bookmark);

  if (index != -1) {
    m_bookmarks.removeAt(index);
    m_settings->setBookmarks(m_bookmarks);
    emit bookmarkRemoved(bookmark);

    if (m_bookmarks.size() == 0) {
      emit emptyChanged();
    }
  }
}

void Bookmarks::add(int sura, int aya) {
  add(serialize(sura, aya));
}

void Bookmarks::remove(int sura, int aya) {
  remove(serialize(sura, aya));
}

QList<uint> Bookmarks::bookmarks() const {
  return m_bookmarks;
}

void Bookmarks::clear() {
  if (!m_bookmarks.isEmpty()) {
    m_settings->setBookmarks(QList<uint>());
    m_bookmarks.clear();
    emit cleared();
    emit emptyChanged();
  }
}

int Bookmarks::sura(uint bookmark) {
  int sura = -1;
  int aya = -1;

  deserialize(bookmark, sura, aya);

  return sura;
}

int Bookmarks::aya(uint bookmark) {
  int sura = -1;
  int aya = -1;

  deserialize(bookmark, sura, aya);

  return aya;
}

void Bookmarks::deserialize(uint bookmark, int& chapter, int& verse) {
  verse = bookmark & 0xffff;
  chapter = (bookmark >> 16);

  // This can happen if we try to serialize and deserialize chapter -1 and verse -1
  // -1 is used to refer to an invalid entity.
  if (verse == (ushort)-1) {
    verse = -1;
  }

  if (chapter == (ushort)-1) {
    chapter = -1;
  }
}
