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

#ifndef TRANSLATION_P_H
#define TRANSLATION_P_H

#include <QObject>
#include "translation.h"
#include <QPair>

class QNetworkReply;
class QTemporaryFile;
class Translations;

class TranslationPrivate : public QObject {
  Q_OBJECT

public:
  TranslationPrivate(Translation::Status status, int tid, Translations *parent);
  ~TranslationPrivate();

  int downloadProgress() const;
  QString error() const;
  Translation::Status status() const;

  bool canDownload() const;
  bool canDestroy() const;

  void add(Translation *translation);
  void remove(Translation *translation);

  bool startDownload(QNetworkReply *reply);
  void stopDownload();

  void remove();

  int tid() const;

  bool isEmpty() const;

private slots:
  void replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void replyError();
  void replyFinished();
  void replyReadyRead();

private:
  void setStatus(Translation::Status status);
  void setDownloadProgress(int downloadProgress);
  bool readData();
  bool install();

  const int m_tid;
  QList<Translation *> m_items;
  Translation::Status m_status;
  int m_downloadProgress;
  QNetworkReply *m_reply;
  QTemporaryFile *m_file;
  QString m_error;
  quint64 m_offset;
  QList<QPair<off_t, size_t> > m_offsets;
  Translations *m_translations;
};

#endif /* TRANSLATION_P_H */
