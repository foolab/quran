/*
 * Copyright (c) 2019-2020 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef MEDIA_SERVICE_H
#define MEDIA_SERVICE_H

#include <QObject>
#include "quran.h"

class MediaState;
class MediaPlayerConfig;
class MediaPlayer;

class MediaService : public QObject {
  Q_OBJECT

  Q_PROPERTY(Quran::PlaybackState state READ state NOTIFY stateChanged);
  Q_PROPERTY(bool isAvailable READ isAvailable NOTIFY isAvailableChanged);

public:
  MediaService(QObject *parent = 0);
  ~MediaService();

  void play(const MediaPlayerConfig& config);

  Quran::PlaybackState state() const;
  bool isAvailable();

public slots:
  void stop();
  void pause();
  void resume();

signals:
  void stateChanged();
  void positionChanged(int chapter, int verse);
  void error();
  void isAvailableChanged();

private:
  MediaState *m_state;
  MediaPlayer *m_player;
};

#endif /* MEDIA_SERVICE_H */
