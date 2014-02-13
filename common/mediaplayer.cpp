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

#include "mediaplayer.h"
#include "mediaplaylist.h"
#include "media.h"
#include <QDebug>
#include "recitation.h"
#include <QThread>
#include "mediadecoder.h"
#include "audiooutput.h"
#include "audiopolicy.h"

MediaPlayer::MediaPlayer(QObject *parent) :
  QObject(parent),
  m_list(0),
  m_index(-1),
  m_decoder(0),
  m_policy(new AudioPolicy(this)) {

  QObject::connect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
  QObject::connect(m_policy, SIGNAL(lost()), this, SLOT(policyLost()));
  QObject::connect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));
}

MediaPlayer::~MediaPlayer() {
  stop();
}

void MediaPlayer::play() {
  if (!m_list) {
    return;
  }

  if (m_decoder) {
    return;
  }

  if (!m_policy->acquire()) {
    emit error();
    return;
  }

  m_decoder = new MediaDecoder(m_list);

  QObject::connect(this, SIGNAL(error()), this, SLOT(stop()));
  QObject::connect(m_decoder, SIGNAL(error()), this, SIGNAL(error()));
  QObject::connect(m_decoder, SIGNAL(audioError()), this, SIGNAL(error()));
  QObject::connect(m_decoder, SIGNAL(audioFinished()), SLOT(stop()));

  QObject::connect(m_decoder, SIGNAL(positionChanged(int, int)),
		   this, SIGNAL(positionChanged(int, int)));

  m_decoder->start();

  emit stateChanged();
}

void MediaPlayer::stop() {
  if (!m_decoder) {
    return;
  }

  // We need to break the loop.
  MediaDecoder *decoder = m_decoder;
  m_decoder = 0;

  decoder->stop();
  decoder->deleteLater();
  decoder = 0;

  m_policy->release();

  emit stateChanged();
}

MediaPlaylist *MediaPlayer::playlist() {
  return m_list;
}

void MediaPlayer::setPlaylist(MediaPlaylist *playlist) {
  if (m_list) {
    delete m_list;
  }

  m_list = playlist;

  if (m_list) {
    QObject::connect(m_list, SIGNAL(cleared()), this, SLOT(listCleared()));
  }

  m_index = -1;
}

Media *MediaPlayer::media() {
  if (m_index == -1) {
    return 0;
  }

  if (!m_list) {
    return 0;
  }

  if (m_index >= m_list->media().size()) {
    return 0;
  }

  return m_list->media().at(m_index);
}

bool MediaPlayer::isPlaying() {
  return m_decoder != 0;
}

void MediaPlayer::listCleared() {
  stop();
  m_index = -1;
}

void MediaPlayer::policyAcquired() {
  if (m_decoder) {
    m_decoder->policyAcquired();
  }
}

void MediaPlayer::policyDenied() {
  if (m_decoder) {
    stop();
    emit error();
  }
}

void MediaPlayer::policyLost() {
  if (m_decoder) {
    stop();
    emit error();
  }
}
