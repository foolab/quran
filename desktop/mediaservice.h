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

class MediaPlayerConfig;
class MediaPlayer;
class FlipSensor;

class MediaService : public QObject {
  Q_OBJECT

  Q_PROPERTY(Quran::PlaybackState state READ state NOTIFY stateChanged);
  Q_PROPERTY(bool isAvailable READ isAvailable NOTIFY isAvailableChanged);
  Q_PROPERTY(bool flipToPause READ isFlipToPauseEnabled WRITE setFlipToPauseEnabled NOTIFY flipToPauseChanged);

public:
  MediaService(QObject *parent = 0);
  ~MediaService();

  void play(const MediaPlayerConfig& config);

  Quran::PlaybackState state() const;
  bool isAvailable();

  bool isFlipToPauseEnabled();
  void setFlipToPauseEnabled(bool enabled);

public slots:
  void stop();
  void pause();
  void resume();

signals:
  void stateChanged();
  void positionChanged(int chapter, int verse);
  void error();
  void isAvailableChanged();
  void flipToPauseChanged();

private:
  void setSensorState();

  MediaPlayer *m_player;
  FlipSensor *m_sensor;
  bool m_flipToPause;
};

#endif /* MEDIA_SERVICE_H */
