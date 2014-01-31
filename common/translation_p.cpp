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

#include "translation_p.h"
#include <QNetworkReply>
#include <QTemporaryFile>
#include "translations.h"
#include "index.h"
#include <QDebug>

TranslationPrivate::TranslationPrivate(Translation::Status status, int tid, Translations *parent)
  : QObject(parent), m_tid(tid), m_status(status),
    m_downloadProgress(0), m_reply(0), m_file(0), m_offset(0), m_translations(parent) {

  m_error = tr("Download failed");
}

TranslationPrivate::~TranslationPrivate() {
  stopDownload();
}

int TranslationPrivate::downloadProgress() const {
  return m_downloadProgress;
}

int TranslationPrivate::tid() const {
  return m_tid;
}

QString TranslationPrivate::error() const {
  return m_error;
}

Translation::Status TranslationPrivate::status() const {
  return m_status;
}

bool TranslationPrivate::isEmpty() const {
  return m_items.isEmpty();
}

bool TranslationPrivate::canDownload() const {
  return m_reply == 0 && m_file == 0
    && (m_status == Translation::None || m_status == Translation::Error);
}

bool TranslationPrivate::canDestroy() const {
  if (!isEmpty()) {
    return false;
  }

  return m_status == Translation::None || m_status == Translation::Installed;
}

void TranslationPrivate::add(Translation *translation) {
  if (m_items.indexOf(translation) == -1) {
    m_items << translation;

    // TODO: bad
    QMetaObject::invokeMethod(translation, "errorChanged", Qt::QueuedConnection);
    QMetaObject::invokeMethod(translation, "downloadProgressChanged", Qt::QueuedConnection);
    QMetaObject::invokeMethod(translation, "statusChanged", Qt::QueuedConnection);
  }
}

void TranslationPrivate::remove(Translation *translation) {
  int index = m_items.indexOf(translation);
  if (index != -1) {
    m_items.takeAt(index);
  }
}

bool TranslationPrivate::startDownload(QNetworkReply *reply) {
  if (!canDownload()) {
    return false;
  }

  QTemporaryFile *file = new QTemporaryFile(this);

  if (!file->open()) {
    delete file;
    return false;
  }

  m_reply = reply;
  m_file = file;

  setDownloadProgress(0);

  m_offset = 0;
  m_offsets.clear();

  m_reply->setParent(this);

  QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
		   this, SLOT(replyDownloadProgress(qint64, qint64)));
  QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
		   this, SLOT(replyError()));
  QObject::connect(reply, SIGNAL(finished()),
		   this, SLOT(replyFinished()));
  QObject::connect(reply, SIGNAL(readyRead()),
		   this, SLOT(replyReadyRead()));

  setStatus(Translation::Downloading);

  return true;
}

void TranslationPrivate::stopDownload() {
  if (!m_reply || !m_file) {
    return;
  }

  m_reply->abort();
  m_reply->deleteLater();
  m_reply = 0;
  delete m_file;
  m_file = 0;

  setStatus(Translation::None);
}

void TranslationPrivate::setStatus(Translation::Status status) {
  if (m_status != status) {
    Translation::Status oldStatus = m_status;

    m_status = status;

    foreach (Translation *translation, m_items) {
      translation->statusChanged();
    }

    m_translations->statusChanged(tid(), oldStatus, m_status);
  }
}

void TranslationPrivate::setDownloadProgress(int downloadProgress) {
  if (m_downloadProgress != downloadProgress) {
    m_downloadProgress = downloadProgress;
    foreach (Translation *translation, m_items) {
      translation->downloadProgressChanged();
    }
  }
}

bool TranslationPrivate::readData() {
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

bool TranslationPrivate::install() {
  QString index = m_translations->index(tid());
  QString data = m_translations->data(tid());

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

void TranslationPrivate::replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
  if (!m_reply || !m_file) {
    return;
  }

  if (bytesTotal == -1) {
    // TODO: hack!
    // 5 MB!
    bytesTotal = 1024 * 1024 * 5;
  }

  int progress = ((bytesReceived * 100)/bytesTotal);

  setDownloadProgress(progress);
}

void TranslationPrivate::replyError() {
  if (!m_reply || !m_file) {
    return;
  }

  m_reply->deleteLater();
  m_reply = 0;
  delete m_file;
  m_file = 0;

  setStatus(Translation::Error);
}

void TranslationPrivate::replyFinished() {
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
  }
}

void TranslationPrivate::replyReadyRead() {
  if (!m_reply || !m_file) {
    return;
  }

  if (!readData()) {
    replyError();
  }
}

void TranslationPrivate::remove() {
  if (m_status == Translation::None) {
    return;
  }

  stopDownload();

  QString index = m_translations->index(tid());
  QString data = m_translations->data(tid());

  if (QFile(index).remove() && QFile(data).remove()) {
    setStatus(Translation::None);
  }
}
