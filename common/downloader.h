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
};

class Download : public QObject {
  Q_OBJECT

public:
  Download(QNetworkReply *reply, QObject *parent = 0);
  ~Download();

  QNetworkReply *reply() const;
  void stop();
  qint64 progress() const;
  qint64 size() const;

signals:
  void progressChanged();
  void sizeChanged();
  void finished();

private slots:
  void handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
  qint64 m_size;
  qint64 m_progress;
  QNetworkReply *m_reply;
};

#endif /* DOWNLOADER_H */
