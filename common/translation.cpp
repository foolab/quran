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

#include "translation.h"
#include "translations.h"
#include "downloader.h"
#include <QLocale>
#include <QTemporaryFile>
#include <QNetworkReply>
#include "index.h"
#include <QQmlInfo>

Translation::Translation(TranslationInfo *info, Translations *parent) :
  QObject(parent),
  m_info(info),
  m_translations(parent),
  m_downloader(0),
  m_download(0),
  m_loaded(false) {

}

Translation::~Translation() {
  delete m_info;
  m_info = 0;
}

bool Translation::isLoaded() const {
  return m_loaded;
}

void Translation::setLoaded(bool loaded) {
  if (m_loaded != loaded) {
    m_loaded = loaded;
    emit loadedChanged();
  }
}

void Translation::setDownloader(Downloader *downloader) {
  m_downloader = downloader;
}

QString Translation::uuid() const {
  return m_info->m_uuid;
}

QString Translation::name() const {
  return m_info->m_name;
}

QString Translation::language() const {
  return QLocale::languageToString(static_cast<QLocale::Language>(m_info->m_language));
}

qint64 Translation::downloadProgress() const {
  return m_download ? m_download->progress() : 0;
}

void Translation::setStatus(Translation::Status status) {
  if (m_info->m_status != status) {
    m_info->m_status = status;
    emit statusChanged();
  }
}

Translation::Status Translation::status() const {
  return m_info->m_status;
}

qint64 Translation::downloadSize() const {
  return m_download ? m_download->size() : 0;
}

bool Translation::startDownload() {
  stopDownload();

  QString url = QString("http://tanzil.net/trans/?transID=%1&type=txt").arg(m_info->m_uuid);
  m_download = m_downloader->get(url);

  m_offsets.clear();

  QObject::connect(m_download, &Download::progressChanged,
		   this, &Translation::downloadProgressChanged);
  QObject::connect(m_download, &Download::sizeChanged,
		   this, &Translation::downloadSizeChanged);

  QObject::connect(m_download, SIGNAL(finished()),
		   this, SLOT(replyFinished()));

  setStatus(Translation::Downloading);

  return true;
}

void Translation::stopDownload() {
  if (status() != Translation::Downloading) {
    return;
  }

  m_download->stop();
  clearDownload();

  setStatus(Translation::None);
}

void Translation::replyError() {
  if (!m_download) {
    return;
  }

  qmlInfo(this) << "Error downloading "
		<< name()
		<< " "
		<< m_download->reply()->errorString();

  clearDownload();

  setStatus(Translation::Error);
}

void Translation::replyFinished() {
  if (!m_download) {
    return;
  }

  if (m_download->reply()->error() != QNetworkReply::NoError) {
    replyError();
    return;
  }

  QTemporaryFile file;
  if (!file.open()) {
    qmlInfo(this) << "Failed to open temporary file" << file.errorString();
    clearDownload();
    setStatus(Translation::Error);
    return;
  }

  if (!readData(file) || !install(file)) {
    replyError();
  } else {
    clearDownload();
    setStatus(Translation::Installed);

    // Emit this after we set the status because Translations object depends on that
    emit installed();
  }
}

bool Translation::readData(QTemporaryFile& file) {
  quint64 offset = 0;

  while (m_download->reply()->canReadLine()) {
    QByteArray data = m_download->reply()->readLine();

    if (file.write(data) != data.size() || file.error() != QFile::NoError) {
      qmlInfo(this) << "Failed to write data to file" << file.errorString();
      return false;
    }

    if (data.startsWith('\n') || data.startsWith('#')) {
      // Empty lines or comments.
      offset += data.size();
    }
    else {
      m_offsets << qMakePair<off_t, size_t>(offset, data.size() - 1);
      offset += data.size();
    }
  }

  return true;
}

bool Translation::install(QTemporaryFile& file) {
  QString index = m_translations->indexPath(uuid());
  QString data = m_translations->dataPath(uuid());

  if (!file.rename(data)) {
    qmlInfo(this) << "Failed to rename file" << file.errorString();
    file.remove();
    return false;
  }

  QMap<QString, QVariant> meta;
  meta["size"] = file.size();

  if (!Index::write(index, m_offsets, meta)) {
    qmlInfo(this) << "Failed to write index file";
    file.remove();
    return false;
  }

  file.setAutoRemove(false);

  return true;
}

void Translation::clearDownload() {
  m_download->deleteLater();
  m_download = 0;

  // And we need to reset those properties
  emit downloadSizeChanged();
  emit downloadProgressChanged();
}
