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

#include "bookmarks.h"
#include "settings.h"

Bookmarks::Bookmarks(Settings *settings, QObject *parent) :
  QObject(parent), m_settings(settings) {

  m_bookmarks = m_settings->bookmarks();
  qSort(m_bookmarks);
}

Bookmarks::~Bookmarks() {
  m_settings = 0;
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

    int index = m_bookmarks.indexOf(bookmark);
    emit bookmarkAdded(bookmark, index);

    if (m_bookmarks.size() == 1) {
      emit emptyChanged();
    }
  }
}

void Bookmarks::removeByIndex(int index) {
  if (index < m_bookmarks.size()) {
    uint bookmark = m_bookmarks[index];
    m_bookmarks.removeAt(index);
    m_settings->setBookmarks(m_bookmarks);
    emit bookmarkRemoved(bookmark, index);

    if (m_bookmarks.size() == 0) {
      emit emptyChanged();
    }
  }
}

void Bookmarks::remove(uint bookmark) {
  int index = m_bookmarks.indexOf(bookmark);

  if (index != -1) {
    m_bookmarks.removeAt(index);
    m_settings->setBookmarks(m_bookmarks);
    emit bookmarkRemoved(bookmark, index);

    if (m_bookmarks.size() == 0) {
      emit emptyChanged();
    }
  }
}

QVariantList Bookmarks::bookmarks() const {
  QVariantList list;

  for (int x = 0; x < m_bookmarks.size(); x++) {
    list << m_bookmarks[x];
  }

  return list;
}

void Bookmarks::clear() {
  m_settings->setBookmarks(QList<uint>());
  m_bookmarks.clear();
  emit cleared();
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

void Bookmarks::deserialize(uint bookmark, int& sura, int& aya) {
  aya = bookmark & 0xffff;

  sura = (bookmark >> 16);
}
