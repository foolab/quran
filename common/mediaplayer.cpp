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

#include "mediaplayer.h"
#include "mediaplaylist.h"
#include "media.h"
#include "mediadecoder.h"
#include "audiooutput.h"
#include "audiopolicy.h"
#include "mediaplayerconfig.h"
#include "mediastate.h"
#include <QQmlInfo>

MediaPlayer::MediaPlayer(MediaState *handler, QObject *parent) :
  QObject(parent),
  m_state(handler),
  m_list(0),
  m_decoder(0),
  m_policy(0),
  m_audio(0) {

  QObject::connect(this, SIGNAL(error()), this, SLOT(pause()));
}

MediaPlayer::~MediaPlayer() {
  stop();

  m_state->save();
}

Quran::PlaybackState MediaPlayer::state() {
  if (!m_list) {
    return Quran::Stopped;
  } else if (!m_audio) {
    return Quran::Playing;
  } else if (m_audio->isPaused()) {
    return Quran::Paused;
  } else {
    return Quran::Playing;
  }
}

void MediaPlayer::policyAcquired() {
  m_audio = AudioOutput::create(this);
  if (!m_audio) {
    qmlInfo(this) << "Failed to create audio output";
    emit error();
    return;
  }

  QObject::connect(m_audio, SIGNAL(positionChanged(int)),
		   this, SLOT(audioPositionChanged(int)));

  QObject::connect(m_audio, SIGNAL(error()), this, SIGNAL(error()));
  QObject::connect(m_audio, &AudioOutput::finished,
		   this, [this]() {
			   m_state->setPosition(0);
			   stop();
			 });

  if (!m_audio->start()) {
    qmlInfo(this) << "Failed to start audio output";
    delete m_audio;
    m_audio = 0;
    emit error();
    return;
  }

  m_decoder->setOutput(m_audio);
}

void MediaPlayer::policyDenied() {
  if (m_decoder) {
    emit error();
  }
}

void MediaPlayer::mediaAvailable(const Media& media) {
  m_decoder->addMedia(media);
}

void MediaPlayer::audioPositionChanged(int index) {
  m_state->setPosition(index);

  int chapter, verse;
  if (m_list->signalMedia(index, chapter, verse)) {
    emit positionChanged(chapter, verse);
  } else {
    emit positionChanged(-1, -1);
  }
}

void MediaPlayer::play(const MediaPlayerConfig& config) {
  if (m_decoder || m_audio || m_list || m_policy) {
    stop();
  }

  if (config.localPath().isEmpty()) {
    qmlInfo(this) << "No recitation set";
    stop();
    emit error();
    return;
  }

  m_state->setConfig(config);

  m_list = new MediaPlaylist(config);

  m_policy = new AudioPolicy(this);
  QObject::connect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
  QObject::connect(m_policy, SIGNAL(lost()), this, SLOT(pause()));
  QObject::connect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));

  if (!m_policy->acquire()) {
    stop();
    emit error();
    return;
  }

  QObject::connect(m_list, SIGNAL(mediaAvailable(const Media&)),
		   this, SLOT(mediaAvailable(const Media&)));

  m_decoder = new MediaDecoder(m_list->dataProvider(), this);

  m_list->start();

  m_decoder->start();

  emit stateChanged();
}

void MediaPlayer::stop() {
  if (m_decoder) {
    m_decoder->stop();
    m_decoder->deleteLater();
    m_decoder = 0;
  }

  if (m_audio) {
    m_audio->stop();
    QObject::disconnect(m_audio, 0, this, 0);

    m_audio->deleteLater();
    m_audio = 0;
  }

  if (m_list) {
    m_list->stop();
    QObject::disconnect(m_list, SIGNAL(mediaAvailable(const Media&)),
			this, SLOT(mediaAvailable(const Media&)));
    m_list->deleteLater();
    m_list = 0;
  }

  if (m_policy) {
    QObject::disconnect(m_policy, 0, this, 0);

    m_policy->release();
    m_policy->deleteLater();
    m_policy = 0;
  }

  emit stateChanged();
}

void MediaPlayer::pause() {
  if (m_audio) {
    m_audio->pause();
    emit stateChanged();
  }
}

void MediaPlayer::resume() {
  if (m_audio) {
    m_audio->resume();
    emit stateChanged();
  }
}
