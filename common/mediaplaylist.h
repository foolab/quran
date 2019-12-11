/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef MEDIA_PLAYLIST_H
#define MEDIA_PLAYLIST_H

#include <QObject>
#include <QQueue>

class MediaPlayerConfig;
class Media;
class Downloader;
class Download;
class RecitationDataProvider;

class MediaPlaylist : public QObject {
  Q_OBJECT

public:
  MediaPlaylist(const MediaPlayerConfig& config, QObject *parent = 0);
  ~MediaPlaylist();

  RecitationDataProvider *dataProvider() const;

  const QList<Media> media() const;

  bool signalMedia(int index, int& chapter, int& verse) const;

  void start();
  void stop();

signals:
  void mediaAvailable(const Media& media);

private slots:
  void replyFinished();

private:
  void addMedia(const Media& media);

  void download();

  RecitationDataProvider *m_dataProvider;
  Downloader *m_downloader;

  QList<Media> m_media;
  QQueue<Media> m_queue;

  QString m_url;
  Download *m_download;
};

#endif /* MEDIA_PLAYLIST_H */
