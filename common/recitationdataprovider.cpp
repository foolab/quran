/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#include "recitationdataprovider.h"
#include <QFileInfo>
#include <QDebug>
#include <QScopedPointer>
#include <QDir>
#include <quazip.h>
#include <quazipfile.h>
#include "media.h"

#define BACKUP_SUFFIX                  "~"

QuaZip *openZekrRecitation(const QString& path);
QString zekrSub(QuaZip *zip);

RecitationDataProvider::RecitationDataProvider(const QString& path) :
  m_path(path) {

  m_zip = openZekrRecitation(m_path);
  m_sub = zekrSub(m_zip);
}

RecitationDataProvider::~RecitationDataProvider() {
  delete m_zip;
  m_zip = 0;
}

QByteArray RecitationDataProvider::data(const Media& media) {
  QString mp3(localPath(media));

  if (m_zip) {
    if (!m_zip->setCurrentFile(mp3)) {
      qWarning() << "Failed to find " << mp3;
      return QByteArray();
    }

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
  } else {
    // TODO: use mmap
    QFile f(mp3);

    if (!f.open(QFile::ReadOnly)) {
      qWarning() << "Failed to open file " << f.fileName() << " " << f.errorString();
      return QByteArray();
    }

    QByteArray data = f.readAll();
    if (f.error() != QFile::NoError) {
      qWarning() << "Failed to read file " << f.fileName() << " " << f.errorString();
    }

    return data;
  }

  // Cosmetic
  return QByteArray();
}

bool RecitationDataProvider::setData(const Media& media, const QByteArray& data) {
  if (isLocal()) {
    qWarning() << "setData should not be called for non-online recitations";
    return false;
  }

  QString mp3(localPath(media));

  QFileInfo inf(mp3);
  if (!inf.dir().mkpath(".")) {
    qWarning() << "Failed to create directory " << inf.dir();
    return false;
  }

  QFile f(QString("%1%2").arg(mp3).arg(BACKUP_SUFFIX));
  if (!f.open(QFile::WriteOnly)) {
    qWarning() << "Failed to open " << f.fileName() << " " << f.errorString();
    return false;
  }

  if (!f.write(data)) {
    qWarning() << "Failed to write " << f.fileName() << " " << f.errorString();
    f.remove();
    return false;
  }

  f.close();

  return QFile::rename(mp3 + BACKUP_SUFFIX, mp3);
}

bool RecitationDataProvider::isLocal() const {
  return m_zip != 0;
}

QString RecitationDataProvider::localPath(const Media& media) {
  if (isLocal()) {
    return QString("%1/%2/%2%3.mp3")
      .arg(m_sub)
      .arg(media.chapter(), 3, 10, QChar('0'))
      .arg(media.verse(), 3, 10, QChar('0'));
  } else {
    return QString("%1/%2/%2%3.mp3")
      .arg(m_path)
      .arg(media.chapter(), 3, 10, QChar('0'))
      .arg(media.verse(), 3, 10, QChar('0'));
  }
}

QuaZip *openZekrRecitation(const QString& path) {
  QFileInfo inf(path);

  if (inf.isFile() && inf.suffix().toLower() == "zip") {
    QScopedPointer<QuaZip> z(new QuaZip(path));
    if (!z->open(QuaZip::mdUnzip)) {
      return 0;
    }

    return z.take();
  }

  return 0;
}

QString zekrSub(QuaZip *zip) {
  if (!zip) {
    return QString();
  }

  for (bool more = zip->goToFirstFile(); more; more = zip->goToNextFile()) {
    QString file = zip->getCurrentFileName();
    if (file.endsWith(".mp3", Qt::CaseInsensitive)) {
      return file.left(file.indexOf("/"));
    }
  }

  if (zip->getZipError() != UNZ_OK) {
    return QString();
  }

  return QString();
}
