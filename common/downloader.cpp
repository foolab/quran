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

QNetworkReply *Downloader::get(const QString& url) {
  return get(QUrl::fromEncoded(url.toUtf8()));
}

QNetworkReply *Downloader::get(const QUrl& url) {
  QNetworkReply *reply = QNetworkAccessManager::get(QNetworkRequest(url));
  QObject::connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)), reply,
		   SLOT(ignoreSslErrors()));

  return reply;
}
