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

#include "bookmarkitem.h"
#include "bookmarks.h"
#include <QDebug>

BookmarkItem::BookmarkItem(QObject *parent) :
  QObject(parent),
  m_bookmarks(0),
  m_bookmark(0),
  m_bookmarked(false) {

}

BookmarkItem::~BookmarkItem() {

}

void BookmarkItem::setBookmarks(Bookmarks *bookmarks) {
  if (m_bookmarks != bookmarks) {

    if (m_bookmarks) {
      QObject::disconnect(m_bookmarks, SIGNAL(bookmarkAdded(uint)), this, SLOT(check()));
      QObject::disconnect(m_bookmarks, SIGNAL(bookmarkRemoved(uint)), this, SLOT(check()));
      QObject::disconnect(m_bookmarks, SIGNAL(cleared()), this, SLOT(check()));
    }

    m_bookmarks = bookmarks;

    if (m_bookmarks) {
      QObject::connect(m_bookmarks, SIGNAL(bookmarkAdded(uint)), this, SLOT(check()));
      QObject::connect(m_bookmarks, SIGNAL(bookmarkRemoved(uint)), this, SLOT(check()));
      QObject::connect(m_bookmarks, SIGNAL(cleared()), this, SLOT(check()));
    }

    emit bookmarksChanged();

    check();
  }
}

Bookmarks *BookmarkItem::bookmarks() const {
  return m_bookmarks;
}

void BookmarkItem::setBookmark(uint bookmark) {
  if (m_bookmark != bookmark) {
    m_bookmark = bookmark;
    emit bookmarkChanged();
  }

  // We do this everytime because our default value for m_bookmark is 0
  check();
}

uint BookmarkItem::bookmark() const {
  return m_bookmark;
}

bool BookmarkItem::isBookmarked() const {
  return m_bookmarked;
}

void BookmarkItem::check() {
  bool bookmarked = false;

  if (!m_bookmarks) {
    goto out;
  }

  bookmarked = m_bookmarks->isBookmarked(m_bookmark);

out:
  if (m_bookmarked != bookmarked) {
    m_bookmarked = bookmarked;
    emit bookmarkedChanged();
  }
}

void BookmarkItem::toggle() {
  if (!m_bookmarks) {
    qCritical() << "m_bookmarks not set";
    return;
  }

  if (isBookmarked()) {
    m_bookmarks->remove(m_bookmark);
  }
  else {
    m_bookmarks->add(m_bookmark);
  }
}
