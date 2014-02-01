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

#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QObject>
#include <gst/gst.h>

class MediaPlaylist;
class Media;

class MediaPlayer : public QObject {
  Q_OBJECT

public:
  MediaPlayer(QObject *parent = 0);
  ~MediaPlayer();

  void play();
  void stop();

  MediaPlaylist *playlist();
  void setPlaylist(MediaPlaylist *playlist);

  Media *media();

  bool isPlaying();

signals:
  void error();
  void stateChanged();
  void mediaChanged();

private slots:
  void listCleared();

private:
  void setNextIndex();
  static void queue_next_uri(GstElement* elem, MediaPlayer *that);
  static gboolean bus_handler(GstBus *bus, GstMessage *message, MediaPlayer *that);
  static void source_setup(GstElement *bin, GstElement *src, MediaPlayer *that);

  GstElement *m_bin;
  MediaPlaylist *m_list;
  int m_index;
  bool m_playing;
  GstElement *m_src;
};

#endif /* MEDIA_PLAYER_H */
