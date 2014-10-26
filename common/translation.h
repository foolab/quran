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

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>
#include <QPair>

class Translations;
class Downloader;
class QNetworkReply;
class QTemporaryFile;

class Translation : public QObject {
  Q_OBJECT

  Q_ENUMS(Status);

  Q_PROPERTY(int tid READ tid CONSTANT);
  Q_PROPERTY(QString uuid READ uuid CONSTANT);
  Q_PROPERTY(QString name READ name CONSTANT);
  Q_PROPERTY(QString language READ language CONSTANT);
  Q_PROPERTY(qint64 downloadSize READ downloadSize NOTIFY downloadSizeChanged);
  Q_PROPERTY(qint64 downloadProgress READ downloadProgress NOTIFY downloadProgressChanged);
  Q_PROPERTY(Status status READ status NOTIFY statusChanged);
  Q_PROPERTY(bool loaded READ isLoaded NOTIFY loadedChanged);

public:
  enum Status {
    None,
    Installed,
    Downloading,
    Error,
  };

  Translation(int tid, const QString& id,
	      const QString& name, int language, Translations *parent = 0);
  ~Translation();

  void setDownloader(Downloader *downloader);

  int tid() const;
  QString uuid() const;
  QString name() const;
  QString language() const;

  qint64 downloadSize() const;
  qint64 downloadProgress() const;

  void setStatus(Status status);
  Status status() const;

  Q_INVOKABLE bool startDownload();

  bool isLoaded() const;
  void setLoaded(bool loaded);

public slots:
  void stopDownload();

signals:
  void statusChanged();
  void downloadProgressChanged();
  void downloadSizeChanged();
  void loadedChanged();
  void installed();

private slots:
  void replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void replyError();
  void replyFinished();
  void replyReadyRead();

private:
  bool readData();
  bool install();

  Translations *m_translations;
  Downloader *m_downloader;
  QNetworkReply *m_reply;
  QTemporaryFile *m_file;
  Status m_status;
  const int m_tid;
  qint64 m_size;
  qint64 m_progress;
  const QString m_name;
  const int m_language;
  const QString m_id;
  bool m_loaded;

  quint64 m_offset;
  QList<QPair<off_t, size_t> > m_offsets;
};

#endif /* TRANSLATION_H */
