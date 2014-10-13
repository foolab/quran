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

#ifndef BOOKMARKS_MODEL_H
#define BOOKMARKS_MODEL_H

#include <QAbstractListModel>

class Bookmarks;

class BookmarksModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Bookmarks *bookmarks READ bookmarks WRITE setBookmarks NOTIFY bookmarksChanged);

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    ChapterRole,
    VerseRole,
  };

  BookmarksModel(QObject *parent = 0);
  ~BookmarksModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Bookmarks *bookmarks() const;
  void setBookmarks(Bookmarks *bookmarks);

signals:
  void bookmarksChanged();

private slots:
  void addId(uint id);
  void removeId(uint id);
  void clearIds();

private:
  void setIds(const QList<uint>& ids);

#ifdef QT_VERSION_5
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif

  Bookmarks *m_bookmarks;
  QList<uint> m_ids;
};

#endif /* BOOKMARKS_MODEL_H */
