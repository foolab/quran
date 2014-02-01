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

class DataProvider;
class Recitation;
class Media;

class MediaPlaylist : public QObject {
  Q_OBJECT

public:
  enum PlayMode {
    PlayPage,
    PlayChapter,
    PlayVerse,
    PlayPart,
  };

  MediaPlaylist(DataProvider *data, QObject *parent = 0);
  ~MediaPlaylist();

  void setRecitation(Recitation *recitation);

  void playPage(int page);
  void playChapter(int chapter);
  void playVerse(int chapter, int verse);
  void playPart(int part);

  PlayMode mode();

  int page();
  int chapter();
  int part();

  QList<Media *> media();

  Recitation *recitation();

signals:
  void cleared();
  void mediaAdded(Media *media);

private:
  void clear();
  void addMedia(Media *media);

  DataProvider *m_data;
  Recitation *m_recitation;

  PlayMode m_mode;

  int m_chapter;
  int m_page;
  int m_part;

  QList<Media *> m_media;
};

#endif /* MEDIA_PLAYLIST_H */
