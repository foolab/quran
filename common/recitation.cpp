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

#include "recitation.h"
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include "quazipfile.h"
#include "media.h"

#define SIMPLE_INFO_FILE               "info.ini"
#define ZEKR_INFO_FILE                 "recitation.properties"
#define ONLINE_INFO_FILE               "data.ini"

Recitation::~Recitation() {

}

QString Recitation::id() const {
  return m_id;
}

QString Recitation::name() const {
  return m_name;
}

QString Recitation::dir() const {
  return m_dir;
}

bool Recitation::isValid() {
  return !m_id.isEmpty() && !m_name.isEmpty() && !m_dir.isEmpty();
}

Recitation::Recitation(const QString& name, const QString& id, const QString& dir)
  : m_name(name), m_id(id), m_dir(dir) {

}

QByteArray Recitation::data(const Media *media) {
  QFile f(media->url().toLocalFile());

  if (!f.open(QFile::ReadOnly)) {
    qWarning() << "Failed to open file" << f.fileName() << f.errorString();
    return QByteArray();
  }

  QByteArray data = f.readAll();
  if (f.error() != QFile::NoError) {
    qWarning() << "Failed to read file" << f.fileName() << f.errorString();
  }

  return data;
}

class RecitationSimple : public Recitation {
public:
  static RecitationSimple *create(const QString& id, const QString& dir) {
    QDir d(dir);
    QString path = d.filePath(SIMPLE_INFO_FILE);

    QFileInfo inf(path);

    if (!inf.exists() || !inf.isReadable()) {
      return 0;
    }

    QString name = QString::fromUtf8(QSettings(path, QSettings::IniFormat).value("recitation/name").toByteArray());
    if (name.isEmpty()) {
      return 0;
    }

    return new RecitationSimple(name, id, dir);
  }

  Media *mediaUrl(int chapter, int verse, int index) {
    QString mp3 = QString("%1/%2%3.mp3").arg(dir()).arg(chapter, 3, 10, QChar('0')).arg(verse, 3, 10, QChar('0'));

    return new Media(this, chapter, verse, index, QUrl::fromLocalFile(mp3));
  }

protected:
  RecitationSimple(const QString& name, const QString& id, const QString& dir) :
    Recitation(name, id, dir) {

  }
};

class RecitationOnline : public Recitation {
public:
  static RecitationOnline *create(const QString& id, const QString& dir) {
    QDir d(dir);
    QString path = d.filePath(ONLINE_INFO_FILE);

    QFileInfo inf(path);

    if (!inf.exists() || !inf.isReadable()) {
      return 0;
    }

    QSettings s(path, QSettings::IniFormat);
    QUrl url = s.value("url/url").toUrl();

    if (url.isEmpty() || !url.isValid()) {
      return 0;
    }

    return new RecitationOnline(id, dir, url);
  }

  Media *mediaUrl(int chapter, int verse, int index) {
    QString mp3 = QString("%1/%2%3.mp3").arg(dir()).arg(chapter, 3, 10, QChar('0')).arg(verse, 3, 10, QChar('0'));
    if (QFile::exists(mp3)) {
      return new Media(this, chapter, verse, index, QUrl::fromLocalFile(mp3));
    }

    QUrl url(QString("%1/%2%3.mp3").arg(m_url.toString()).arg(chapter, 3, 10, QChar('0')).arg(verse, 3, 10, QChar('0')));

    return new Media(this, chapter, verse, index, url);
  }

protected:
  RecitationOnline(const QString& name, const QString& dir, const QUrl& url) :
    Recitation(name, name, dir) {
    m_url = url;
  }

private:
  QUrl m_url;
};

class RecitationZekr : public Recitation {
public:
  static RecitationZekr *create(const QString& id, const QString& dir) {
    QDir d(dir);
    QString path = d.filePath(ZEKR_INFO_FILE);

    QFileInfo inf(path);

    if (!inf.exists() || !inf.isReadable()) {
      return 0;
    }

    // Look for:
    // audio.id => mp3 subdirectory
    // audio.language => use that to extract a localized audio.reciter
    // audio.reciter => fallback name

    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
      return 0;
    }

    QString name, subdir;

    if (!RecitationZekr::info(file, name, subdir)) {
      return 0;
    }

    return new RecitationZekr(name, id, QDir(dir).filePath(subdir));
  }

  Media *mediaUrl(int chapter, int verse, int index) {
    QString mp3 = QString("%1/%2/%2%3.mp3").arg(dir()).arg(chapter, 3, 10, QChar('0')).arg(verse, 3, 10, QChar('0'));

    return new Media(this, chapter, verse, index, QUrl::fromLocalFile(mp3));
  }

  static bool info(QIODevice& file, QString& name, QString& subdir) {
    QByteArray lang;

    QMap<QByteArray, QByteArray> map;

    while (!file.atEnd()) {
      QByteArray a = file.readLine();
      a.chop(1); // Remove terminating \n
      QList<QByteArray> as = a.split('=');
      if (as.size() != 2) {
	continue;
      }

      QByteArray key = as[0].trimmed();
      QByteArray val = as[1].trimmed();
      map.insert(key, val);
    }

    if (map.contains("audio.id")) {
      subdir = QString::fromUtf8(map["audio.id"]);
    }

    if (map.contains("audio.language")) {
      lang = map["audio.language"];
    }

    if (name.isEmpty() && map.contains("audio.reciter")) {
      name = QString::fromUtf8(map["audio.reciter"]);
    }

    if (!lang.isEmpty() && map.contains("audio.reciter." + lang)) {
      name = QString::fromUtf8(map["audio.reciter." + lang]);
    }

    if (name.isEmpty() || subdir.isEmpty()) {
      return false;
    }

    return true;
  }

private:
  RecitationZekr(const QString& name, const QString& id, const QString& dir)
    : Recitation(name, id, dir) {
  }
};

class RecitationZekrZip : public Recitation {
public:
  static RecitationZekrZip *create(const QString& id, const QString& dir) {
    if (!dir.endsWith(".zip", Qt::CaseInsensitive)) {
      return 0;
    }

    QuaZip *zip = new QuaZip(dir);
    if (!zip->open(QuaZip::mdUnzip)) {
      delete zip;
      return 0;
    }

    zip->setCurrentFile(ZEKR_INFO_FILE);

    QuaZipFile file(zip);
    if (!file.open(QIODevice::ReadOnly)) {
      delete zip;
      return 0;
    }

    QString name, subdir;

    if (!RecitationZekr::info(file, name, subdir)) {
      delete zip;
      return 0;
    }

    return new RecitationZekrZip(name, id, QFileInfo(dir).baseName(), zip);
  }

  Media *mediaUrl(int chapter, int verse, int index) {
    QString mp3 = QString("%1/%2/%2%3.mp3").arg(dir()).arg(chapter, 3, 10, QChar('0')).arg(verse, 3, 10, QChar('0'));

    return new Media(this, chapter, verse, index, QUrl(mp3));
  }

  QByteArray data(const Media *media) {
    QString mp3 = QString("%1/%2/%2%3.mp3").arg(dir()).arg(media->chapter(), 3, 10, QChar('0')).arg(media->verse(), 3, 10, QChar('0'));

    m_zip->setCurrentFile(mp3);
    QuaZipFile file(m_zip);
    if (!file.open(QIODevice::ReadOnly)) {
      qWarning() << "Failed to open file" << mp3;
      return QByteArray();
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()) {
      qWarning() << "Failed to read file" << mp3;
    }

    return data;
  }

private:
  RecitationZekrZip(const QString& name, const QString& id, const QString& dir, QuaZip *zip)
    : Recitation(name, id, dir),
      m_zip(zip) {

  }

  ~RecitationZekrZip() {
    delete m_zip;
    m_zip = 0;
  }

  QuaZip *m_zip;
};

Recitation *Recitation::create(const QString& id, const QString& dir) {
  Recitation *r = RecitationSimple::create(id, dir);

  if (!r) {
    r = RecitationZekr::create(id, dir);
  }

  if (!r) {
    r = RecitationZekrZip::create(id, dir);
  }

  if (!r) {
    return 0;
  }

  if (r->isValid()) {
    return r;
  }

  delete r;
  return 0;
}
