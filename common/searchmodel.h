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

#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include <QAbstractListModel>

class SearchModelPrivate;

class SearchModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(bool error READ error NOTIFY errorChanged);
  Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged);

public:
  enum Roles {
    ChapterRole = Qt::UserRole + 1,
    VerseRole,
  };

  SearchModel(QObject *parent = 0);
  ~SearchModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  bool error() const;
  QString errorString() const;

public slots:
  void setQuery(const QString& query, bool matchWholeWords);

signals:
  void errorChanged();

private:
  void clearError();
  void setError(const QString& err);

  void clearModel();
  void setResults(const QList<QPair<int, int> >& results);

#ifdef QT_VERSION_5
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif

  SearchModelPrivate *d_ptr;
};

#endif /* SEARCH_MODEL_H */
