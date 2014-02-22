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

#ifndef MEDIA_PLAYLIST_H
#define MEDIA_PLAYLIST_H

#include <QObject>
#include <QQueue>

class DataProvider;
class Recitation;
class Media;
class Recitation;
class Downloader;
class QNetworkReply;

class MediaPlaylist : public QObject {
  Q_OBJECT

public:
  static MediaPlaylist *partList(DataProvider *data, Recitation *recitation,
				 Downloader *downloader, int part, QObject *parent = 0);
  static MediaPlaylist *pageList(DataProvider *data, Recitation *recitation,
				 Downloader *downloader, int page, QObject *parent = 0);
  static MediaPlaylist *verseList(DataProvider *data, Recitation *recitation,
				  Downloader *downloader, int chapter, int verse,
				  QObject *parent = 0);
  static MediaPlaylist *chapterList(DataProvider *data, Recitation *recitation,
				    Downloader *downloader, int chapter, QObject *parent = 0);

  ~MediaPlaylist();

  Recitation *recitation();

  const QList<Media> media() const;

  bool signalMedia(int index, int& chapter, int& verse) const;

  void start();
  void stop();

signals:
  void mediaAvailable(const Media& media);

private slots:
  void replyFinished();

private:
  MediaPlaylist(DataProvider *data, Recitation *recitation,
		Downloader *downloader, QObject *parent = 0);

  void playPage(int page);
  void playChapter(int chapter);
  void playVerse(int chapter, int verse);
  void playPart(int part);

  void addMedia(const Media& media);

  void download();

  enum PlayMode {
    PlayPage,
    PlayChapter,
    PlayVerse,
    PlayPart,
  };

  DataProvider *m_data;
  Recitation *m_recitation;
  Downloader *m_downloader;

  PlayMode m_mode;

  QList<Media> m_media;
  QQueue<Media> m_queue;

  int m_playingId;

  QNetworkReply *m_reply;
};

#endif /* MEDIA_PLAYLIST_H */
