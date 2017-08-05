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

#include "recitation.h"
#include "media.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QQmlInfo>
#include <QSettings>
#include <QTextCodec>
#include <quazip.h>
#include <quazipfile.h>

#define BACKUP_SUFFIX                  "~"
#define SIMPLE_INFO_FILE               "info.ini"
#define ZEKR_INFO_FILE                 "recitation.properties"
#define ONLINE_INFO_FILE               "data.ini"

Recitation::Recitation(RecitationInfo *info, QObject *parent) :
  QObject(parent),
  m_info(info),
  m_loaded(false),
  m_zip(0) {

}

Recitation::~Recitation() {
  delete m_info;
  m_info = 0;

  delete m_zip;
  m_zip = 0;
}

RecitationInfo *Recitation::guessType(const QString& recitationDir) {
  // Simple translation:
  {
    QSettings s(QString("%1%2").arg(recitationDir).arg(SIMPLE_INFO_FILE), QSettings::IniFormat);
    s.setIniCodec(QTextCodec::codecForName("UTF-8"));
    QString name = s.value("recitation/name").toString();
    if (!name.isEmpty()) {
      RecitationInfo *info = new RecitationInfo;
      info->m_type = Recitation::Simple;
      info->m_status = Recitation::Installed;
      info->m_name = name;
      info->m_dir = recitationDir;

      return info;
    }
  }

  // Online
  {
    QSettings s(QString("%1%2").arg(recitationDir).arg(ONLINE_INFO_FILE), QSettings::IniFormat);
    s.setIniCodec(QTextCodec::codecForName("UTF-8"));
    QUrl url = s.value("info/url").toUrl();
    QString name = s.value("info/name").toString();
    if (!url.isEmpty() && url.isValid() && !name.isEmpty()) {
      bool enabled = s.value("info/enabled", false).toBool();

      RecitationInfo *info = new RecitationInfo;
      info->m_type = Recitation::Online;
      info->m_status = enabled ? Recitation::Installed : Recitation::None;
      info->m_name = name;
      info->m_quality = s.value("info/quality").toString();
      info->m_dir = recitationDir;

      return info;
    }
  }

  // zekr uncompressed
  {
    QFile file(QString("%1%2").arg(recitationDir).arg(ZEKR_INFO_FILE));
    if (file.exists() && file.open(QFile::ReadOnly)) {
      RecitationInfo *info = parseZekr(&file, Recitation::Unzipped);
      if (info) {
	info->m_dir = recitationDir;
      }

      return info;
    }
  }

  // zipped
  {
    if (recitationDir.endsWith(".zip", Qt::CaseInsensitive)) {
      QuaZip z(recitationDir);
      if (!z.open(QuaZip::mdUnzip)) {
	return 0;
      }

      if (!z.setCurrentFile(ZEKR_INFO_FILE)) {
	return 0;
      }

      QuaZipFile file(&z);
      if (!file.open(QIODevice::ReadOnly)) {
	return 0;
      }

      RecitationInfo *info = parseZekr(&file, Recitation::Zipped);
      if (info) {
	info->m_dir = recitationDir;
      }

      return info;
    }
  }

  return 0;
}

RecitationInfo *Recitation::parseZekr(QIODevice *dev, const Type& type) {
  QByteArray lang;
  QString name, subdir, quality;
  QMap<QByteArray, QByteArray> map;

  while (!dev->atEnd()) {
    QByteArray a = dev->readLine();
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

  if (map.contains("audio.quality")) {
    quality = QString::fromUtf8(map["audio.quality"]);
  }

  if (name.isEmpty() || subdir.isEmpty()) {
    return 0;
  }

  RecitationInfo *info = new RecitationInfo;
  info->m_type = type;
  info->m_name = name;
  info->m_quality = quality;
  info->m_status = Recitation::Installed;
  info->m_subdir = subdir;

  return info;
}

int Recitation::rid() const {
  return m_info->m_id;
}

QString Recitation::uuid() const {
  return m_info->m_uuid;
}

QString Recitation::name() const {
  return m_info->m_name;
}

QString Recitation::quality() const {
  return m_info->m_quality;
}

Recitation::Type Recitation::type() const {
  return m_info->m_type;
}

Recitation::Status Recitation::status() const {
  return m_info->m_status;
}

void Recitation::setStatus(Recitation::Status status) {
  if (m_info->m_status != status) {
    m_info->m_status = status;
    emit statusChanged();
  }
}

QUrl Recitation::playBackUrl(const Media& media) {
  QString mp3 = QString("%1/%2/%2%3.mp3")
    .arg(m_info->m_dir)
    .arg(media.chapter(), 3, 10, QChar('0'))
    .arg(media.verse(), 3, 10, QChar('0'));

  return QUrl::fromLocalFile(mp3);
}

QUrl Recitation::downloadUrl(const Media& media) {
  if (!m_info->m_url.isValid()) {
    return QUrl();
  }

  return QUrl(QString("%1/%2%3.mp3")
	      .arg(m_info->m_url.toString())
	      .arg(media.chapter(), 3, 10, QChar('0'))
	      .arg(media.verse(), 3, 10, QChar('0')));
}

QByteArray Recitation::data(const Media& media) {
  if (m_info->m_type == Zipped) {
    if (!m_zip) {
      m_zip = new QuaZip(m_info->m_dir);
      if (!m_zip->open(QuaZip::mdUnzip)) {
	delete m_zip;
	m_zip = 0;
	qmlInfo(this) << "Failed to open " << m_info->m_dir;
	return QByteArray();
      }
    }

    QString mp3 = QString("%1/%2/%2%3.mp3")
      .arg(m_info->m_subdir)
      .arg(media.chapter(), 3, 10, QChar('0'))
      .arg(media.verse(), 3, 10, QChar('0'));

    if (!m_zip->setCurrentFile(mp3)) {
      delete m_zip;
      m_zip = 0;
      qmlInfo(this) << "Failed to find " << mp3;
      return QByteArray();
    }

    QuaZipFile file(m_zip);
    if (!file.open(QIODevice::ReadOnly)) {
      delete m_zip;
      m_zip = 0;
      qmlInfo(this) << "Failed to open file" << mp3;
      return QByteArray();
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()) {
      delete m_zip;
      m_zip = 0;
      qmlInfo(this) << "Failed to read file" << mp3;
    }

    return data;
  }

  // TODO: use mmap
  QFile f(playBackUrl(media).toLocalFile());

  if (!f.open(QFile::ReadOnly)) {
    qmlInfo(this) << "Failed to open file " << f.fileName() << " " << f.errorString();
    return QByteArray();
  }

  QByteArray data = f.readAll();
  if (f.error() != QFile::NoError) {
    qmlInfo(this) << "Failed to read file " << f.fileName() << " " << f.errorString();
  }

  return data;
}

bool Recitation::setData(const Media& media, const QByteArray& data) {
  if (m_info->m_type != Online) {
    qmlInfo(this) << "setData should not be called for non-online recitations";
    return false;
  }

  QString mp3 = playBackUrl(media).toLocalFile();

  QFileInfo inf(mp3);
  if (!inf.dir().mkpath(".")) {
    qmlInfo(this) << "Failed to create directory " << inf.dir();
    return false;
  }

  QFile f(QString("%1%2").arg(mp3).arg(BACKUP_SUFFIX));
  if (!f.open(QFile::WriteOnly)) {
    qmlInfo(this) << "Failed to open " << f.fileName() << " " << f.errorString();
    return false;
  }

  if (!f.write(data)) {
    qmlInfo(this) << "Failed to write " << f.fileName() << " " << f.errorString();
    f.remove();
    return false;
  }

  f.close();

  return QFile::rename(mp3 + BACKUP_SUFFIX, mp3);
}

bool Recitation::enable() {
  if (m_info->m_type != Online) {
    qmlInfo(this) << "Only online recitations can be enabled";
    return false;
  }

  QString filePath = path(ONLINE_INFO_FILE);
  QSettings s(filePath, QSettings::IniFormat);
  s.setIniCodec(QTextCodec::codecForName("UTF-8"));
  s.setValue("info/url", m_info->m_url);
  s.setValue("info/name", m_info->m_name);
  s.setValue("info/quality", m_info->m_quality);
  s.setValue("info/enabled", true);

  s.sync();

  if (s.status() == QSettings::NoError) {
    setStatus(Installed);
    emit enabled();
    return true;
  }

  QFile::remove(filePath);

  return false;
}

bool Recitation::disable() {
  if (m_info->m_type != Online) {
    qmlInfo(this) << "Only online recitations can be enabled";
    return false;
  }

  QSettings s(path(ONLINE_INFO_FILE), QSettings::IniFormat);
  s.setIniCodec(QTextCodec::codecForName("UTF-8"));
  s.setValue("info/url", m_info->m_url);
  s.setValue("info/name", m_info->m_name);
  s.setValue("info/quality", m_info->m_quality);
  s.setValue("info/enabled", false);

  s.sync();

  if (s.status() == QSettings::NoError) {
    setStatus(None);
    emit disabled();
    return true;
  }

  return false;
}

/*
bool Recitation::clear() {

}
*/

QString Recitation::path(const QString fileName) const {
  return QString("%1%2").arg(m_info->m_dir).arg(fileName);
}

bool Recitation::isLoaded() const {
  return m_loaded;
}

void Recitation::setLoaded(bool loaded) {
  if (m_loaded != loaded) {
    m_loaded = loaded;
    emit loadedChanged();
  }
}
