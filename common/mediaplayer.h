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

#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QObject>

class MediaPlaylist;
class Media;
class MediaDecoder;
class AudioPolicy;
class AudioOutput;
class Recitation;
class Downloader;

class MediaPlayer : public QObject {
  Q_OBJECT

  Q_ENUMS(PlayType);

  Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged);
  Q_PROPERTY(bool paused READ isPaused NOTIFY pausedChanged);

public:
  enum PlayType {
    PlayVerse,
    PlayPage,
    PlayChapter,
    PlayPart,
  };

  MediaPlayer(QObject *parent = 0);
  ~MediaPlayer();

  Recitation *recitation() const;
  void setRecitation(Recitation *recitation);

  Q_INVOKABLE bool play(const PlayType& type, uint id);
  Q_INVOKABLE bool playRange(uint fromChapter, uint fromVerse, uint toChapter, uint toVerse);

  bool isPlaying() const;
  bool isPaused() const;

public slots:
  void stop();
  void pause();
  void resume();

signals:
  void playingChanged();
  void pausedChanged();
  void dataChanged();
  void positionChanged(int chapter, int verse);
  void error();

private slots:
  void policyAcquired();
  void policyLost();
  void policyDenied();
  void mediaAvailable(const Media& media);
  void audioPositionChanged(int index);

private:
  bool play();

  MediaPlaylist *m_list;
  MediaDecoder* m_decoder;
  AudioPolicy *m_policy;
  AudioOutput *m_audio;
  Downloader *m_downloader;
  Recitation *m_recitation;
};

#endif /* MEDIA_PLAYER_H */
