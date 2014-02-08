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

#ifndef BOOKMARK_ITEM_H
#define BOOKMARK_ITEM_H

#include <QObject>

class Bookmarks;

class BookmarkItem : public QObject {
  Q_OBJECT
  Q_PROPERTY(Bookmarks *bookmarks READ bookmarks WRITE setBookmarks NOTIFY bookmarksChanged);
  Q_PROPERTY(uint bookmark READ bookmark WRITE setBookmark NOTIFY bookmarkChanged);
  Q_PROPERTY(bool isBookmarked READ isBookmarked NOTIFY bookmarkedChanged);

public:
  BookmarkItem(QObject *parent = 0);
  ~BookmarkItem();

  void setBookmarks(Bookmarks *bookmarks);
  Bookmarks *bookmarks() const;

  void setBookmark(uint bookmark);
  uint bookmark() const;

  bool isBookmarked() const;

public slots:
  void toggle();

signals:
  void bookmarksChanged();
  void bookmarkChanged();
  void bookmarkedChanged();

private slots:
  void check();

private:
  Bookmarks *m_bookmarks;
  uint m_bookmark;
  bool m_bookmarked;
};

#endif /* BOOKMARK_ITEM_H */
