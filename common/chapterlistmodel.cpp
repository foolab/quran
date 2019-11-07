/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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

#include "chapterlistmodel.h"
#include "metadata.h"
#include "chapterinfo.h"

ChapterListModel::ChapterListModel(QObject *parent) :
  QAbstractListModel(parent) {

}

ChapterListModel::~ChapterListModel() {

}

int ChapterListModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return MAX_CHAPTER + 1;
  }

  return 0;
}

QVariant ChapterListModel::data(const QModelIndex& index, int role) const {
  if (index.row() >= 0 && index.row() <= MAX_CHAPTER) {
    switch (role) {
    case NameRole:
      return ChapterInfo(index.row()).fullName();

    case LengthRole:
      return ChapterInfo(index.row()).length();

    default:
      break;
    }
  }

  return QVariant();
}

QHash<int, QByteArray> ChapterListModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  roles[LengthRole] = "length";

  return roles;
}
