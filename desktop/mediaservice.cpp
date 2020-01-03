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

#include "mediaservice.h"
#include "mediaplayer.h"

MediaService::MediaService(QObject *parent) :
  QObject(parent),
  m_player(new MediaPlayer(this)) {

  QObject::connect(m_player, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));
  QObject::connect(m_player, SIGNAL(positionChanged(int, int)), this, SIGNAL(positionChanged(int, int)));
  QObject::connect(m_player, SIGNAL(error()), this, SIGNAL(error()));
}

MediaService::~MediaService() {
  delete m_player;
  m_player = 0;
}

void MediaService::play(const MediaPlayerConfig& config) {
  m_player->play(config);
}

Quran::PlaybackState MediaService::state() const {
  return m_player->state();
}

bool MediaService::isAvailable() {
  return true;
}

void MediaService::stop() {
  return m_player->stop();
}

void MediaService::pause() {
  return m_player->pause();
}

void MediaService::resume() {
  return m_player->resume();
}
