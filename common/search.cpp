#include "search.h"
#include "../sqlite/sqlite3.h"
#include "bookmarks.h"
#include <QDebug>

#define QUERY "SELECT chapter, verse FROM search WHERE text MATCH ?1 ORDER BY chapter, verse ASC;"

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
    emit errorOpen();
    return false;
  }

  return true;
}

QVariantList Search::search(const QString& query) {
  if (!enable()) {
    return QVariantList();
  }

  QByteArray s(QUERY);

  sqlite3_stmt *stmt = NULL;

  int ret = sqlite3_prepare_v2(d_ptr->db,              /* Database handle */
			       s.constData(),          /* SQL statement, UTF-8 encoded */
			       s.size(),               /* Maximum length of zSql in bytes. */
			       &stmt,                  /* OUT: Statement handle */
			       NULL                    /* OUT: Pointer to unused portion of zSql */
			       );

  if (ret != SQLITE_OK) {
    emit error();
    return QVariantList();
  }

  QByteArray q(query.toUtf8());

  ret = sqlite3_bind_text(stmt, 1, q.constData(), q.size(), SQLITE_STATIC);

  if (ret != SQLITE_OK) {
    emit error();
    sqlite3_finalize(stmt);
    return QVariantList();
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

  emit error();

  return QVariantList();
}
