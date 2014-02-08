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

#include "bookmarksmodel.h"
#include "bookmarks.h"
#include <QDebug>

BookmarksModel::BookmarksModel(QObject *parent) :
  QAbstractListModel(parent),
  m_bookmarks(0) {

  QHash<int, QByteArray> roles;
  roles[IdRole] = "bookmark";
  roles[ChapterRole] = "chapter";
  roles[VerseRole] = "verse";

  setRoleNames(roles);
}

BookmarksModel::~BookmarksModel() {

}

int BookmarksModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_ids.size();
  }

  return 0;
}

QVariant BookmarksModel::data(const QModelIndex& index, int role) const {
  if ((role == IdRole || role == ChapterRole || role == VerseRole) && index.row() < m_ids.size()) {
    if (role == IdRole) {
      return m_ids[index.row()];
    }
    else if (role == ChapterRole) {
      return m_bookmarks->sura(m_ids[index.row()]);
    }
    else if (role == VerseRole) {
      return m_bookmarks->aya(m_ids[index.row()]);
    }
  }

  return QVariant();
}

Bookmarks *BookmarksModel::bookmarks() const {
  return m_bookmarks;
}

void BookmarksModel::setBookmarks(Bookmarks *bookmarks) {
  if (m_bookmarks != bookmarks) {

    if (m_bookmarks) {
      QObject::disconnect(m_bookmarks, SIGNAL(bookmarkAdded(uint)), this, SLOT(addId(uint)));
      QObject::disconnect(m_bookmarks, SIGNAL(bookmarkRemoved(uint)), this, SLOT(removeId(uint)));
      QObject::disconnect(m_bookmarks, SIGNAL(cleared()), this, SLOT(clearIds()));
    }

    m_bookmarks = bookmarks;

    if (m_bookmarks) {
      QObject::connect(m_bookmarks, SIGNAL(bookmarkAdded(uint)), this, SLOT(addId(uint)));
      QObject::connect(m_bookmarks, SIGNAL(bookmarkRemoved(uint)), this, SLOT(removeId(uint)));
      QObject::connect(m_bookmarks, SIGNAL(cleared()), this, SLOT(clearIds()));
    }

    emit bookmarksChanged();

    setIds(m_bookmarks->bookmarks());
  }
}

void BookmarksModel::clearIds() {
  if (!m_ids.isEmpty()) {
    beginRemoveRows(QModelIndex(), 0, m_ids.size() - 1);
    m_ids.clear();
    endRemoveRows();
  }
}

void BookmarksModel::addId(uint id) {
  if (m_ids.indexOf(id) != -1) {
    qCritical() << "id already known" << id;
    return;
  }

  beginInsertRows(QModelIndex(), m_ids.size(), m_ids.size());
  m_ids << id;
  endInsertRows();
}

void BookmarksModel::removeId(uint id) {
  int index = m_ids.indexOf(id);
  if (index == -1) {
    qCritical() << "unknown id" << id;
    return;
  }

  beginRemoveRows(QModelIndex(), index, index);
  m_ids.removeAt(index);
  endRemoveRows();
}

void BookmarksModel::setIds(const QList<uint>& ids) {
  clearIds();

  if (!ids.isEmpty()) {
    beginInsertRows(QModelIndex(), 0, ids.size() - 1);
    m_ids = ids;
    endInsertRows();
  }
}
