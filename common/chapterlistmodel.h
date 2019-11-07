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

#ifndef CHAPTER_LIST_MODEL_H
#define CHAPTER_LIST_MODEL_H

#include <QAbstractListModel>

class ChapterListModel : public QAbstractListModel {
  Q_OBJECT

public:
  enum {
    NameRole = Qt::UserRole,
    LengthRole = Qt::UserRole + 1,
  };

  ChapterListModel(QObject *parent = 0);
  ~ChapterListModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  QHash<int, QByteArray> roleNames() const;
};

#endif /* CHAPTER_LIST_MODEL_H */
