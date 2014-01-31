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

#include "search.h"
#include <sqlite3.h>
#include "bookmarks.h"
#include <QDebug>

#define DB_PATH DATA_DIR "/search.db"

#define MATCH_PART_QUERY "SELECT chapter, verse FROM search WHERE text LIKE ?1 ORDER BY chapter, verse ASC;"

#define MATCH_ALL_QUERY "SELECT chapter, verse FROM search WHERE text MATCH ?1 ORDER BY chapter, verse ASC;"

class SearchPrivate {
public:
  sqlite3 *db;
  QString path;
};

Search::Search(QObject *parent) : QObject(parent), d_ptr(new SearchPrivate) {
  d_ptr->db = 0;
  d_ptr->path = DB_PATH;
}

Search::~Search() {
  disable();

  delete d_ptr; d_ptr = 0;
}

void Search::disable() {
  if (!d_ptr->db) {
    sqlite3_close(d_ptr->db);
    d_ptr->db = 0;
  }
}

bool Search::enable() {
  if (d_ptr->db) {
    return true;
  }

  if (sqlite3_open_v2(d_ptr->path.toLocal8Bit().data(),
		      &d_ptr->db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
    return false;
  }

  return true;
}

/*
 * First item -1 ? Failed to open
 * First item -2 ? Failed to search
 * Empty ? No results
 */
QVariantList Search::search(const QString& query, bool matchWholeWords) {
  if (!enable()) {
    return QVariantList() << -1;
  }

  QByteArray s(matchWholeWords ? MATCH_ALL_QUERY : MATCH_PART_QUERY);

  sqlite3_stmt *stmt = NULL;

  int ret = sqlite3_prepare_v2(d_ptr->db,              /* Database handle */
			       s.constData(),          /* SQL statement, UTF-8 encoded */
			       s.size(),               /* Maximum length of zSql in bytes. */
			       &stmt,                  /* OUT: Statement handle */
			       NULL                    /* OUT: Pointer to unused portion of zSql */
			       );

  if (ret != SQLITE_OK) {
    return QVariantList() << -2;
  }

  QByteArray q;
  if (matchWholeWords) {
    q = query.toUtf8();
  }
  else {
    q = "%" + query.toUtf8() + "%";
  }

  ret = sqlite3_bind_text(stmt, 1, q.constData(), q.size(), SQLITE_STATIC);

  if (ret != SQLITE_OK) {
    sqlite3_finalize(stmt);
    return QVariantList() << -2;
  }

  ret = sqlite3_step(stmt);

  QVariantList res;

  while (ret == SQLITE_ROW) {
    int chapter = sqlite3_column_int(stmt, 0);
    int verse = sqlite3_column_int(stmt, 1);

    res << Bookmarks::serialize(chapter, verse);

    ret = sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);

  if (ret == SQLITE_DONE) {
    return res;
  }

  return QVariantList() << -2;
}
