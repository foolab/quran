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
  m_playing(false),
  m_decoderThread(new QThread(this)),
  m_audioThread(new QThread(this)),
  m_decoder(0),
  m_audio(0),
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

  if (m_playing) {
    return;
  }

  if (!m_policy->acquire()) {
    emit error();
    return;
  }

  m_decoder = new MediaDecoder(m_list);
  m_decoder->moveToThread(m_decoderThread);
  QObject::connect(m_decoder, SIGNAL(error()), this, SIGNAL(error()));
  QObject::connect(m_decoder, SIGNAL(error()), m_decoderThread, SLOT(quit()));
  QObject::connect(m_decoder, SIGNAL(finished()), m_decoderThread, SLOT(quit()));
  QObject::connect(m_decoderThread, SIGNAL(started()), m_decoder, SLOT(process()));

  m_audio = new AudioOutput;
  m_audio->moveToThread(m_audioThread);
  QObject::connect(m_audio, SIGNAL(error()), this, SIGNAL(error()));
  QObject::connect(m_audio, SIGNAL(error()), m_audioThread, SLOT(quit()));
  QObject::connect(m_audio, SIGNAL(finished()), m_audioThread, SLOT(quit()));
  QObject::connect(m_audioThread, SIGNAL(started()), m_audio, SLOT(process()));

  QObject::connect(m_decoder, SIGNAL(play(AudioBuffer *)), m_audio, SLOT(play(AudioBuffer *)));

  QObject::connect(m_audio, SIGNAL(positionChanged(int, int, int)),
		   this, SIGNAL(positionChanged(int, int, int)));

  QObject::connect(m_audio, SIGNAL(finished()), this, SLOT(stop()));
  QObject::connect(m_audio, SIGNAL(error()), this, SLOT(stop()));
  QObject::connect(m_decoder, SIGNAL(error()), this, SLOT(stop()));

  m_playing = true;

  m_decoderThread->start();
  m_audioThread->start();

  emit stateChanged();
}

void MediaPlayer::stop() {
  if (!m_playing) {
    return;
  }

  if (m_decoder) {
    m_decoder->finish();
  }

  if (m_audio) {
    m_audio->finish();
  }

  m_decoderThread->quit();
  m_audioThread->quit();

  if (m_audio) {
    m_audio->deleteLater();
  }

  if (m_decoder) {
    m_decoder->deleteLater();
  }

  m_playing = false;

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
  return m_playing;
}

void MediaPlayer::listCleared() {
  stop();
  m_index = -1;
}

void MediaPlayer::policyAcquired() {
  if (m_playing) {
    m_audio->policyAcquired();
  }
}

void MediaPlayer::policyDenied() {
  if (m_playing) {
    stop();
    emit error();
  }
}

void MediaPlayer::policyLost() {
  if (m_playing) {
    stop();
    emit error();
  }
}
