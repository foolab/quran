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
#include "mediastate.h"
#include "flipsensor.h"

MediaService::MediaService(QObject *parent) :
  QObject(parent),
  m_state(new MediaState),
  m_player(new MediaPlayer(m_state, this)),
  m_sensor(new FlipSensor(this)),
  m_flipToPause(false) {

  QObject::connect(m_player, &MediaPlayer::stateChanged,
		   this, [this]() {
			   setSensorState();
			   emit stateChanged();
			 });

  QObject::connect(m_player, SIGNAL(positionChanged(int, int)),
		   this, SIGNAL(positionChanged(int, int)));
  QObject::connect(m_player, &MediaPlayer::error, [this]() {
						    pause();
						    emit error();
						  });
  QObject::connect(m_player, SIGNAL(policyLost()), this, SLOT(pause()));
  QObject::connect(m_sensor, SIGNAL(flipped()), this, SLOT(pause()));
}

MediaService::~MediaService() {
  delete m_player;
  m_player = 0;

  delete m_state;
  m_state = 0;

  delete m_sensor;
  m_sensor = 0;
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

bool MediaService::isFlipToPauseEnabled() {
  return m_flipToPause;
}

void MediaService::setFlipToPauseEnabled(bool enabled) {
  if (m_flipToPause != enabled) {
    m_flipToPause = enabled;

    setSensorState();

    emit flipToPauseChanged();
  }
}

void MediaService::setSensorState() {
  if (m_flipToPause && m_player->state() == Quran::Playing) {
    m_sensor->setActive(true);
  } else {
    m_sensor->setActive(false);
  }
}
