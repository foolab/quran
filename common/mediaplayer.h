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

#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <QObject>
#include "quran.h"

class MediaState;
class MediaPlaylist;
class Media;
class MediaDecoder;
class AudioPolicy;
class AudioOutput;
class MediaPlayerConfig;

class MediaPlayer : public QObject {
  Q_OBJECT

  Q_PROPERTY(Quran::PlaybackState state READ state NOTIFY stateChanged);

public:
  MediaPlayer(MediaState *state, QObject *parent = 0);
  ~MediaPlayer();

  Q_INVOKABLE Quran::PlaybackState state();

public slots:
  void play(const MediaPlayerConfig& config);
  void stop();
  void pause();
  void resume();

signals:
  void stateChanged();
  void positionChanged(int chapter, int verse);
  void error();

private slots:
  void policyAcquired();
  void policyDenied();
  void mediaAvailable(const Media& media);
  void audioPositionChanged(int index);

private:
  MediaState *m_state;
  MediaPlaylist *m_list;
  MediaDecoder* m_decoder;
  AudioPolicy *m_policy;
  AudioOutput *m_audio;
};

#endif /* MEDIA_PLAYER_H */
