/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef MEDIA_PLAYBACK_H
#define MEDIA_PLAYBACK_H

#include <QObject>

class MediaPlayer;
class Recitation;

class MediaPlayback : public QObject {
  Q_OBJECT

  Q_ENUMS(PlayType);

  Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged);
  Q_PROPERTY(bool paused READ isPaused NOTIFY pausedChanged);

public:
  MediaPlayback(QObject *parent = 0);
  ~MediaPlayback();

  enum PlayType {
    PlayVerse,
    PlayPage,
    PlayChapter,
    PlayPart,
  };

  Q_INVOKABLE bool play(const PlayType& type, uint id);
  Q_INVOKABLE bool playRange(uint fromChapter, uint fromVerse, uint toChapter, uint toVerse);

  bool isPlaying() const;
  bool isPaused() const;

  Recitation *recitation() const;
  void setRecitation(Recitation *recitation);

public slots:
  void stop();
  void pause();
  void resume();

signals:
  void playingChanged();
  void pausedChanged();
  void positionChanged(int chapter, int verse);
  void error();

private:
  MediaPlayer *m_player;
  Recitation *m_recitation;
};

#endif /* MEDIA_PLAYBACK_H */
