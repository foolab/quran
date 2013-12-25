#include "search.h"
#include <sqlite3.h>
#include "bookmarks.h"
#include <QDebug>

#define MATCH_PART_QUERY "SELECT chapter, verse FROM search WHERE text LIKE ?1 ORDER BY chapter, verse ASC;"

#define MATCH_ALL_QUERY "SELECT chapter, verse FROM search WHERE text MATCH ?1 ORDER BY chapter, verse ASC;"

class SearchPrivate {
public:
  sqlite3 *db;
  QString path;
};

Search::Search(const QString& path, QObject *parent) : QObject(parent), d_ptr(new SearchPrivate) {
  d_ptr->db = 0;
  d_ptr->path = path;
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
