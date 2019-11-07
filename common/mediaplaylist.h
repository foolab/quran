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

class Recitation;
class Media;
class Recitation;
class Downloader;
class Download;

class MediaPlaylist : public QObject {
  Q_OBJECT

public:
  static MediaPlaylist *partList(Recitation *recitation,
				 Downloader *downloader, uint part, QObject *parent = 0);
  static MediaPlaylist *pageList(Recitation *recitation,
				 Downloader *downloader, uint page, QObject *parent = 0);
  static MediaPlaylist *verseList(Recitation *recitation,
				  Downloader *downloader, uint serialized,
				  QObject *parent = 0);
  static MediaPlaylist *chapterList(Recitation *recitation,
				    Downloader *downloader, uint chapter, QObject *parent = 0);

  static MediaPlaylist *rangeList(Recitation *recitation,
				  Downloader *downloader,
				  uint fromChapter, uint fromVerse,
				  uint toChapter, uint toVerse,
				  QObject *parent = 0);

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
  MediaPlaylist(Recitation *recitation,
		Downloader *downloader, QObject *parent = 0);

  void playPage(int page);
  void playChapter(int chapter);
  void playVerse(int chapter, int verse);
  void playPart(int part);
  void playRange(uint fromChapter, uint fromVerse, uint toChapter, uint toVerse);

  void addMedia(const Media& media);

  void download();

  enum PlayMode {
    PlayPage,
    PlayChapter,
    PlayVerse,
    PlayPart,
  };

  Recitation *m_recitation;
  Downloader *m_downloader;

  QList<Media> m_media;
  QQueue<Media> m_queue;

  Download *m_download;
};

#endif /* MEDIA_PLAYLIST_H */
