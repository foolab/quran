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
  m_decoder(0),
  m_policy(new AudioPolicy(this)) {

  QObject::connect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
  QObject::connect(m_policy, SIGNAL(lost()), this, SLOT(policyLost()));
  QObject::connect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));

  QObject::connect(this, SIGNAL(error()), this, SLOT(stop()), Qt::QueuedConnection);
}

MediaPlayer::~MediaPlayer() {
  stop();
}

void MediaPlayer::start(MediaPlaylist *list) {
  m_list = list;
  m_list->start();

  if (m_decoder) {
    return;
  }

  if (!m_policy->acquire()) {
    emit error();
    return;
  }

  m_decoder = new MediaDecoder(m_list);

  QObject::connect(m_decoder, SIGNAL(error()), this, SIGNAL(error()));
  QObject::connect(m_decoder, SIGNAL(audioError()), this, SIGNAL(error()));
  QObject::connect(m_decoder, SIGNAL(audioFinished()), this, SLOT(stop()), Qt::QueuedConnection);

  QObject::connect(m_decoder, SIGNAL(positionChanged(int, int)),
		   this, SIGNAL(positionChanged(int, int)));

  m_list->start();

  m_decoder->start();

  emit stateChanged();
}

void MediaPlayer::stop() {
  // We need to break the loop.
  if (m_decoder) {
    MediaDecoder *decoder = m_decoder;
    m_decoder = 0;

    decoder->stop();
    delete decoder;
    decoder = 0;
  }

  if (m_policy) {
    m_policy->release();
  }

  if (m_list) {
    m_list->stop();
    delete m_list;
    m_list = 0;

    emit stateChanged();
  }
}

bool MediaPlayer::isPlaying() const {
  return m_list != 0;
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
