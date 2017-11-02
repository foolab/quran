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

#include "searchmodel.h"
#include "sqlite3.h"
#include <QDebug>
#include "normalize.h"

#ifdef ANDROID
#include "sqlite-ndk/sources/sqlite3ndk.h"
#define DB_PATH "search.db"
#define VFS SQLITE_NDK_VFS_NAME
#else
#define DB_PATH DATA_DIR "/data/search.db"
#define VFS NULL
#endif

#define MATCH_PART_QUERY "SELECT chapter, verse FROM search WHERE text LIKE ?1 ORDER BY chapter, verse ASC;"

#define MATCH_ALL_QUERY "SELECT chapter, verse FROM search WHERE text MATCH ?1 ORDER BY chapter, verse ASC;"

class SearchModelPrivate {
public:
  SearchModelPrivate() : db(0) { }
  sqlite3 *db;
  QString error;
  QList<QPair<int, int> > results;
};

SearchModel::SearchModel(QObject *parent) :
  QAbstractListModel(parent), d_ptr(new SearchModelPrivate) {

}

SearchModel::~SearchModel() {
  if (d_ptr->db) {
    sqlite3_close(d_ptr->db);
    d_ptr->db = 0;
  }

  delete d_ptr;
  d_ptr = 0;
}

int SearchModel::rowCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : d_ptr->results.size();
}

QVariant SearchModel::data(const QModelIndex& index, int role) const {
  if (index.row() >= d_ptr->results.size()) {
    return QVariant();
  }

  switch (role) {
  case ChapterRole:
    return d_ptr->results[index.row()].first;
  case VerseRole:
    return d_ptr->results[index.row()].second;
  default:
    break;
  }

  return QVariant();
}

bool SearchModel::error() const {
  return d_ptr->error.size() > 0;
}

QString SearchModel::errorString() const {
  return d_ptr->error;
}

void SearchModel::setQuery(const QString& query, bool matchWholeWords) {
  clearModel();

  int err = SQLITE_OK;
  if (!d_ptr->db) {
    err = sqlite3_open_v2(DB_PATH, &d_ptr->db, SQLITE_OPEN_READONLY, VFS);
  }

  if (err != SQLITE_OK || !d_ptr->db) {
    setError(tr("Failed to open search database."));
    return;
  }

  QByteArray s(matchWholeWords ? MATCH_ALL_QUERY : MATCH_PART_QUERY);

  sqlite3_stmt *stmt = NULL;

  err = sqlite3_prepare_v2(d_ptr->db,              /* Database handle */
			   s.constData(),          /* SQL statement, UTF-8 encoded */
			   s.size(),               /* Maximum length of zSql in bytes. */
			   &stmt,                  /* OUT: Statement handle */
			   NULL                    /* OUT: Pointer to unused portion of zSql */
			   );

  if (err != SQLITE_OK) {
    setError(tr("Error while searching."));
    return;
  }

  QByteArray q;
  if (matchWholeWords) {
    q = Normalize::normalize(query).toUtf8();
  }
  else {
    q = "%" + Normalize::normalize(query).toUtf8() + "%";
  }

  err = sqlite3_bind_text(stmt, 1, q.constData(), q.size(), SQLITE_STATIC);

  if (err != SQLITE_OK) {
    sqlite3_finalize(stmt);
    setError(tr("Error while searching."));
    return;
  }

  err = sqlite3_step(stmt);

  QList<QPair<int, int> > results;
  while (err == SQLITE_ROW) {
    int chapter = sqlite3_column_int(stmt, 0);
    int verse = sqlite3_column_int(stmt, 1);
    results << qMakePair<int, int>(chapter, verse);
    err = sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);

  if (err == SQLITE_DONE) {
    setResults(results);
  }
  else {
    setError(tr("Error while searching."));
  }
}

void SearchModel::clearError() {
  if (d_ptr->error.size() > 0) {
    d_ptr->error.clear();
    emit errorChanged();
  }
}

void SearchModel::setError(const QString& err) {
  d_ptr->error = err;
  emit errorChanged();
}

void SearchModel::clearModel() {
  if (!d_ptr->results.isEmpty()) {
    beginRemoveRows(QModelIndex(), 0, d_ptr->results.size() - 1);
    d_ptr->results.clear();
    endRemoveRows();
  }

  clearError();
}

void SearchModel::setResults(const QList<QPair<int, int> >& results) {
  if (results.isEmpty()) {
    setError(tr("No results found."));
  }
  else {
    beginInsertRows(QModelIndex(), 0, results.size() - 1);
    d_ptr->results = results;
    endInsertRows();
  }
}

QHash<int, QByteArray> SearchModel::roleNames() const {

  QHash<int, QByteArray> roles;
  roles[ChapterRole] = "chapter";
  roles[VerseRole] = "verse";

  return roles;
}
