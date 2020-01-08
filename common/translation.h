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

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>
#include <QPair>

class Translations;
class Downloader;
class Download;
class TranslationInfo;
class QTemporaryFile;

class Translation : public QObject {
  Q_OBJECT

  Q_ENUMS(Status);

  Q_PROPERTY(QString uuid READ uuid CONSTANT);
  Q_PROPERTY(QString name READ name CONSTANT);
  Q_PROPERTY(QString language READ language CONSTANT);
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

  Translation(TranslationInfo *info, Translations *parent = 0);
  ~Translation();

  void setDownloader(Downloader *downloader);

  QString uuid() const;
  QString name() const;
  QString language() const;

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
  void loadedChanged();
  void installed();

private slots:
  void replyFinished();

private:
  bool readData(QTemporaryFile& file);
  bool install(QTemporaryFile& file);
  void replyError();
  void clearDownload();

  TranslationInfo *m_info;
  Translations *m_translations;
  Downloader *m_downloader;
  Download *m_download;

  bool m_loaded;

  QList<QPair<off_t, size_t> > m_offsets;
};

class TranslationInfo {
public:
  Translation::Status m_status;
  QString m_name;
  int m_language;
  QString m_uuid;
};

#endif /* TRANSLATION_H */
