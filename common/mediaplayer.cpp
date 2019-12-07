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

#include "mediaplayer.h"
#include "mediaplaylist.h"
#include "media.h"
#include "recitation.h"
#include "mediadecoder.h"
#include "audiooutput.h"
#include "audiopolicy.h"
#include "downloader.h"
#include <QQmlInfo>

MediaPlayer::MediaPlayer(QObject *parent) :
  QObject(parent),
  m_list(0),
  m_decoder(0),
  m_policy(0),
  m_audio(0),
  m_downloader(new Downloader(this)),
  m_recitation(0) {

  QObject::connect(this, SIGNAL(error()), this, SLOT(stop()));
}

MediaPlayer::~MediaPlayer() {
  stop();
}

bool MediaPlayer::isPlaying() const {
  return m_list != 0;
}

bool MediaPlayer::isPaused() const {
  return m_audio ? m_audio->isPaused() : false;
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
  QObject::connect(m_audio, SIGNAL(finished()), this, SLOT(stop()));

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

void MediaPlayer::policyLost() {
  if (m_decoder) {
    emit error();
  }
}

void MediaPlayer::mediaAvailable(const Media& media) {
  m_decoder->addMedia(media);
}

void MediaPlayer::audioPositionChanged(int index) {
  int chapter, verse;
  if (m_list->signalMedia(index, chapter, verse)) {
    emit positionChanged(chapter, verse);
  } else {
    emit positionChanged(-1, -1);
  }
}

void MediaPlayer::setRecitation(Recitation *recitation) {
  if (m_recitation) {
    stop();
  }

  m_recitation = recitation;
}

Recitation *MediaPlayer::recitation() const {
  return m_recitation;
}

bool MediaPlayer::play(const PlayType& type, uint id) {
  stop();

  if (!m_recitation) {
    qmlInfo(this) << "No recitation set";
    return false;
  }

  switch (type) {
  case PlayVerse:
    m_list = MediaPlaylist::verseList(m_recitation, m_downloader, id, this);
    break;

  case PlayPage:
    m_list = MediaPlaylist::pageList(m_recitation, m_downloader, id, this);
    break;

  case PlayChapter:
    m_list = MediaPlaylist::chapterList(m_recitation, m_downloader, id, this);
    break;

  case PlayPart:
    m_list = MediaPlaylist::partList(m_recitation, m_downloader, id, this);
    break;

  default:
    qmlInfo(this) << "Unknown play type" << type;
    return false;
  }

  return play();
}

bool MediaPlayer::playRange(uint fromChapter, uint fromVerse, uint toChapter, uint toVerse) {
  stop();

  if (!m_recitation) {
    qmlInfo(this) << "No recitation set";
    return false;
  }

  if (fromChapter == toChapter) {
    if (fromVerse > toVerse) {
      return false;
    }
  }

  if (fromChapter > toChapter) {
    return false;
  }

  m_list = MediaPlaylist::rangeList(m_recitation, m_downloader, fromChapter, fromVerse, toChapter, toVerse, this);
  return play();
}

bool MediaPlayer::play() {
  m_policy = new AudioPolicy(this);
  QObject::connect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
  QObject::connect(m_policy, SIGNAL(lost()), this, SLOT(policyLost()));
  QObject::connect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));

  if (!m_policy->acquire()) {
    delete m_list;
    m_list = 0;
    emit error();
    return false;
  }

  QObject::connect(m_list, SIGNAL(mediaAvailable(const Media&)),
		   this, SLOT(mediaAvailable(const Media&)));

  m_decoder = new MediaDecoder(this);

  m_list->start();

  m_decoder->start();

  emit playingChanged();

  return true;
}

void MediaPlayer::stop() {
  if (m_decoder) {
    m_decoder->stop();
    m_decoder->deleteLater();
    m_decoder = 0;
  }

  if (m_audio) {
    if (m_audio->isPaused()) {
      emit pausedChanged();
    }

    m_audio->stop();
    QObject::disconnect(m_audio, SIGNAL(positionChanged(int)),
			this, SLOT(audioPositionChanged(int)));

    QObject::disconnect(m_audio, SIGNAL(error()), this, SIGNAL(error()));
    QObject::disconnect(m_audio, SIGNAL(finished()), this, SLOT(stop()));

    m_audio->deleteLater();
    m_audio = 0;
  }

  if (m_list) {
    m_list->stop();
    QObject::disconnect(m_list, SIGNAL(mediaAvailable(const Media&)),
			this, SLOT(mediaAvailable(const Media&)));
    m_list->deleteLater();
    m_list = 0;

    emit playingChanged();
  }

  if (m_policy) {
    QObject::disconnect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
    QObject::disconnect(m_policy, SIGNAL(lost()), this, SLOT(policyLost()));
    QObject::disconnect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));

    m_policy->release();
    m_policy->deleteLater();
    m_policy = 0;
  }
}

void MediaPlayer::pause() {
  if (m_audio) {
    m_audio->pause();
    emit pausedChanged();
  }
}

void MediaPlayer::resume() {
  if (m_audio) {
    m_audio->resume();
    emit pausedChanged();
  }
}
