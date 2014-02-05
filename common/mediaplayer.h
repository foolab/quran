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
#include <QPointer>

class MediaPlaylist;
class Media;
class QThread;
class MediaDecoder;
class AudioOutput;

class MediaPlayer : public QObject {
  Q_OBJECT

public:
  MediaPlayer(QObject *parent = 0);
  ~MediaPlayer();

  void play();

  bool isPlaying() const;

  MediaPlaylist *playlist();
  void setPlaylist(MediaPlaylist *playlist);

  Media *media();

  bool isPlaying();

public slots:
  void stop();

signals:
  void error();
  void stateChanged();
  void positionChanged(int chapter, int verse, int index);

private slots:
  void listCleared();

private:
  MediaPlaylist *m_list;
  int m_index;
  bool m_playing;

  QThread *m_decoderThread;
  QThread *m_audioThread;
  QPointer<MediaDecoder> m_decoder;
  QPointer<AudioOutput> m_audio;
};

#endif /* MEDIA_PLAYER_H */
