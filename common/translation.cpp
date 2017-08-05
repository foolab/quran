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
  m_reply(0),
  m_file(0),
  m_size(0),
  m_progress(0),
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
  return m_progress;
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
  return m_size;
}

bool Translation::startDownload() {
  stopDownload();

  QTemporaryFile *file = new QTemporaryFile(this);

  if (!file->open()) {
    delete file;
    return false;
  }

  m_file = file;

  QString url = QString("http://tanzil.net/trans/?transID=%1&type=txt").arg(m_info->m_uuid);
  m_reply = m_downloader->get(url);
  m_reply->setParent(this);

  m_offset = 0;
  m_offsets.clear();

  QObject::connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)),
		   this, SLOT(replyDownloadProgress(qint64, qint64)));
  QObject::connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
		   this, SLOT(replyError()));
  QObject::connect(m_reply, SIGNAL(finished()),
		   this, SLOT(replyFinished()));
  QObject::connect(m_reply, SIGNAL(readyRead()),
		   this, SLOT(replyReadyRead()));

  setStatus(Translation::Downloading);

  return true;
}

void Translation::stopDownload() {
  if (status() != Translation::Downloading) {
    return;
  }

  m_reply->abort();
  m_reply->deleteLater();
  m_reply = 0;
  delete m_file;
  m_file = 0;

  setStatus(Translation::None);
}

void Translation::replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
  if (!m_reply || !m_file) {
    return;
  }

  if (bytesTotal == -1) {
    // We can try to get it from the HTTP content length header
    QVariant val = m_reply->header(QNetworkRequest::ContentLengthHeader);
    if (!val.isValid()) {
      // HACK: Let's hardcode an arbitrary value (3MB).
      bytesTotal = 1024 * 1024 * 3;
    } else {
      bytesTotal = val.value<qint64>();
    }
  }

  if (bytesTotal < bytesReceived) {
    bytesTotal = bytesReceived;
  }

  if (bytesTotal != m_size) {
    m_size = bytesTotal;
    emit downloadSizeChanged();
  }

  if (bytesReceived != m_progress) {
    m_progress = bytesReceived;
    emit downloadProgressChanged();
  }
}

void Translation::replyError() {
  if (!m_reply || !m_file) {
    return;
  }

  qmlInfo(this) << "Error downloading " << name() << " " << m_reply->errorString();

  m_reply->deleteLater();
  m_reply = 0;
  delete m_file;
  m_file = 0;

  setStatus(Translation::Error);
}

void Translation::replyFinished() {
  if (!m_reply || !m_file) {
    return;
  }

  if (m_reply->error() != QNetworkReply::NoError) {
    replyError();
  }
  else if (!readData()) {
    replyError();
  }
  else if (!install()) {
    replyError();
  }
  else {
    delete m_file;
    m_file = 0;

    m_reply->deleteLater();
    m_reply = 0;

    setStatus(Translation::Installed);

    // Emit this after we set the status because Translations object depends on that
    emit installed();
  }
}

void Translation::replyReadyRead() {
  if (!m_reply || !m_file) {
    return;
  }

  if (!readData()) {
    replyError();
  }
}

bool Translation::readData() {
  if (!m_reply || !m_file) {
    return false;
  }

  while (m_reply->canReadLine()) {
    QByteArray data = m_reply->readLine();

    if (m_reply->error() != QNetworkReply::NoError) {
      return false;
    }

    if (m_file->write(data) != data.size() || m_file->error() != QFile::NoError) {
      return false;
    }

    if (data.startsWith('\n') || data.startsWith('#')) {
      // Empty lines or comments.
      m_offset += data.size();
    }
    else {
      m_offsets << qMakePair<off_t, size_t>(m_offset, data.size() - 1);
      m_offset += data.size();
    }
  }

  return true;
}

bool Translation::install() {
  QString index = m_translations->indexPath(uuid());
  QString data = m_translations->dataPath(uuid());

  if (!m_file->rename(data)) {
    m_file->remove();
    return false;
  }

  QMap<QString, QVariant> meta;
  meta["size"] = m_file->size();

  if (!Index::write(index, m_offsets, meta)) {
    m_file->remove();
    return false;
  }

  m_file->setAutoRemove(false);

  return true;
}

