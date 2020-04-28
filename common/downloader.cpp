/*
 * Copyright (c) 2011-2020 Mohammed Sameer <msameer@foolab.org>.
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
#include <QTimer>
#include <QDebug>
#include "networkmanager.h"

#define MAX_DOWNLOAD_RETRY      3
#define DOWNLOAD_RETRY_WAIT     1000 // 1 second in ms

Downloader::Downloader(QObject *parent) :
  QNetworkAccessManager(parent),
  m_manager(new NetworkManager) {

}

Downloader::~Downloader() {
  delete m_manager;
  m_manager = 0;
}

Download *Downloader::get(const QString& url) {
  return get(QUrl::fromEncoded(url.toUtf8()));
}

Download *Downloader::get(const QUrl& url) {
  return new Download(url, this, this);
}

void Downloader::begin() {
  m_manager->ref();
}

void Downloader::end() {
  m_manager->unref();
}

Download::Download(const QUrl& url, Downloader *downloader, QObject *parent) :
  QObject(parent),
  m_downloader(downloader),
  m_progress(0),
  m_reply(0),
  m_url(url),
  m_trials(0) {

  m_downloader->begin();

  download();
}

Download::~Download() {
  m_downloader->end();
  stop();
}

void Download::download() {
  ++m_trials;

  QNetworkReply *reply = m_downloader->get(QNetworkRequest(m_url));

  setReply(reply);
}

void Download::setReply(QNetworkReply *reply) {
  if (m_reply) {
    QObject::disconnect(m_reply, 0, this, 0);
    m_reply->abort();
    m_reply->deleteLater();
    m_reply = 0;
  }

  m_reply = reply;

  if (m_reply) {
    QObject::connect(m_reply, &QNetworkReply::sslErrors, m_reply,
		     qOverload<>(&QNetworkReply::ignoreSslErrors));

    QObject::connect(m_reply, &QNetworkReply::finished, this, &Download::downloadFinished);

    QObject::connect(m_reply, &QNetworkReply::downloadProgress,
		     this, &Download::handleDownloadProgress);

    m_reply->setParent(this);
  }
}

void Download::stop() {
  setReply(0);
}

void Download::handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
  if (bytesTotal == 0 || bytesTotal == -1) {
    return;
  }

  qint64 progress = (bytesReceived * 100) / bytesTotal;
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

void Download::downloadFinished() {
  if (m_trials > MAX_DOWNLOAD_RETRY) {
    emit finished();
    return;
  }

  if (m_reply->error() != QNetworkReply::NoError) {
    emit finished();
    return;
  }

  QVariant v = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

  if (m_reply->error() == QNetworkReply::NoError && !v.isValid()) {
    emit finished();
    return;
  }

  m_url = v.toUrl();

  QTimer::singleShot(m_trials * DOWNLOAD_RETRY_WAIT, this, &Download::download);
}
