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

#include "downloader.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

Downloader::Downloader(QObject *parent) :
  QNetworkAccessManager(parent) {

}

Downloader::~Downloader() {

}

Download *Downloader::get(const QString& url) {
  return get(QUrl::fromEncoded(url.toUtf8()));
}

Download *Downloader::get(const QUrl& url) {
  QNetworkReply *reply = QNetworkAccessManager::get(QNetworkRequest(url));

  return new Download(reply);
}

Download::Download(QNetworkReply *reply, QObject *parent) :
  QObject(parent),
  m_size(0),
  m_progress(0),
  m_reply(reply) {

  QObject::connect(m_reply, SIGNAL(sslErrors(const QList<QSslError>&)), m_reply,
		   SLOT(ignoreSslErrors()));

  QObject::connect(m_reply, &QNetworkReply::finished, this, &Download::finished);

  QObject::connect(m_reply, &QNetworkReply::downloadProgress,
		   this, &Download::handleDownloadProgress);
}

Download::~Download() {
  stop();
}

void Download::stop() {
  if (m_reply) {
    QObject::disconnect(m_reply, 0, this, 0);
    m_reply->deleteLater();
    m_reply = 0;
  }
}

void Download::handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
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
    emit sizeChanged();
  }

  qint64 progress = (bytesReceived * 100) / m_size;
  if (progress != m_progress) {
    m_progress = progress;
    emit progressChanged();
  }
}

QNetworkReply *Download::reply() const {
  return m_reply;
}

qint64 Download::progress() const {
  return m_progress;
}

qint64 Download::size() const {
  return m_size;
}
