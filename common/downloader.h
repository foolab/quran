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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QNetworkAccessManager>

class QUrl;
class Download;

class Downloader : public QNetworkAccessManager {
  Q_OBJECT

public:
  Downloader(QObject *parent = 0);
  ~Downloader();

  Download *get(const QString& url);
  Download *get(const QUrl& url);

  using QNetworkAccessManager::get;
};

class Download : public QObject {
  Q_OBJECT

public:
  Download(const QUrl& url, Downloader *downloader, QObject *parent = 0);
  ~Download();

  QNetworkReply *reply() const;
  void stop();
  qint64 progress() const;

signals:
  void progressChanged();
  void finished();

private slots:
  void handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void downloadFinished();

private slots:
  void download();

private:
  void setReply(QNetworkReply *reply);

  Downloader *m_downloader;
  qint64 m_progress;
  QNetworkReply *m_reply;
  const QUrl m_url;
  int m_trials;
};

#endif /* DOWNLOADER_H */
