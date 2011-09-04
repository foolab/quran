/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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


#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include "../../sqlite/sqlite3.h"

#define QUERY "insert into search values(?1, ?2, ?3);"

class Sura {
public:
  int index;
  QStringList ayat;
};
QList<Sura> suras;

bool read(const QString& in) {
  QFile f(in);

  if (!f.open(QFile::ReadOnly)) {
    qCritical() << "Failed to open" << f.fileName() << f.errorString();
    return false;
  }

  QXmlStreamReader reader(&f);

  while (!reader.atEnd()) {
    reader.readNext();

    if (reader.isStartElement() && reader.name() == "sura") {
      QXmlStreamAttributes attrs(reader.attributes());

      if (!attrs.hasAttribute("index") || !attrs.hasAttribute("name")) {
	qCritical() << "Missing attributes for sura";
	return false;
      }

      Sura sura;
      sura.index = attrs.value("index").toString().toInt() - 1;

      suras << sura;
    }


    else if (reader.isStartElement() && reader.name() == "aya") {
      QXmlStreamAttributes attrs(reader.attributes());

      if (!attrs.hasAttribute("index") || !attrs.hasAttribute("text")) {
	qCritical() << "Missing attributes for aya";
	return false;
      }

      suras.last().ayat.append(attrs.value("text").toString().trimmed());
    }
  }

  return true;
}

bool write(const QString& out) {
  QString file = out + "-";

  QFile::remove(out);
  QFile::remove(file);

  sqlite3 *db;

  if (sqlite3_open_v2(file.toLocal8Bit().data() ,
		      &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
    qCritical() << "Failed to open db";
    return false;
  }

  if (sqlite3_exec(db, "CREATE VIRTUAL TABLE search USING fts4(chapter int, verse int, text TEXT);",
		   NULL, NULL, NULL) != SQLITE_OK) {
    qCritical() << "Failed to create table" << sqlite3_errmsg(db);
    return false;
  }

  if (sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL)  != SQLITE_OK) {
    qCritical() << "Failed to begin transaction" << sqlite3_errmsg(db);
    return false;
  }

  QByteArray s(QUERY);

  for (int x = 0; x < suras.size(); x++) {
    //    qCritical() << "Chapter" << x;

    for (int i = 0; i < suras[x].ayat.size(); i++) {

      sqlite3_stmt *stmt = NULL;

      int ret = sqlite3_prepare_v2(db,              /* Database handle */
				   s.constData(),   /* SQL statement, UTF-8 encoded */
				   s.size(),        /* Maximum length of zSql in bytes. */
				   &stmt,           /* OUT: Statement handle */
				   NULL             /* OUT: Pointer to unused portion of zSql */
				   );

      if (ret != SQLITE_OK) {
	qCritical() << "Failed to prepare statement" << sqlite3_errmsg(db);
	return false;
      }

      int sura = x;
      int aya = i;
      QString text = suras[x].ayat[i];

      QByteArray t(text.toUtf8());

      if (sqlite3_bind_int(stmt, 1, sura) != SQLITE_OK ||
	  sqlite3_bind_int(stmt, 2, aya) != SQLITE_OK ||
	  sqlite3_bind_text(stmt, 3, t.constData(), t.size(), SQLITE_STATIC)
	  != SQLITE_OK) {
	qCritical() << "Failed to bind statement" << sqlite3_errmsg(db);
	return false;
      }

      if (sqlite3_step(stmt) != SQLITE_DONE) {
	qCritical() << "Failed to execute statement" << sqlite3_errmsg(db);
	return false;
      }

      if (sqlite3_finalize(stmt) != SQLITE_OK) {
	qCritical() << "Failed to finalize statement" << sqlite3_errmsg(db);
	return false;
      }
    }
  }

  if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
    qCritical() << "Failed to commit" << sqlite3_errmsg(db);
    return false;
  }

  if (sqlite3_close(db) != SQLITE_OK) {
    qCritical() << "Failed to close database" << sqlite3_errmsg(db);
    return false;
  }

  return QFile::rename(file, out);
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  Q_UNUSED(app);

  if (argc != 3) {
    qCritical() << "Usage: <text.xml> <out.db>";
    return 1;
  }

  if (!read(argv[1])) {
    return 1;
  }

  if (!write(argv[2])) {
    return 1;
  }

  return 0;
}
