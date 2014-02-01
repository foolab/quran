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

#include "recitations.h"
#include "recitation.h"
#include <QDebug>
#include "settings.h"
#include "mediaplayer.h"
#include "media.h"
#include "audiopolicy.h"
#include "mediaplaylist.h"

Recitations::Recitations(QObject *parent)
  : QObject(parent),
    m_settings(0),
    m_data(0),
    m_player(0),
    m_playlist(0),
    m_recitation(0),
    m_current(0),
    m_policy(new AudioPolicy(this)),
    m_play(false),
    m_chapter(-1),
    m_verse(-1){

  QObject::connect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
  QObject::connect(m_policy, SIGNAL(lost()), this, SLOT(policyLost()));
  QObject::connect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));
}

Recitations::~Recitations() {
  qDeleteAll(m_installed.values());
  m_installed.clear();

  if (m_player) {
    m_player->stop();
  }

  m_policy->release();

  delete m_playlist;
  m_playlist = 0;

  delete m_player;
  m_player = 0;

  m_recitation = 0;
}

Settings *Recitations::settings() const {
  return m_settings;
}

void Recitations::setSettings(Settings *settings) {
  if (m_settings != settings) {
    m_settings = settings;
    emit settingsChanged();
  }
}

DataProvider *Recitations::data() const {
  return m_data;
}

void Recitations::setData(DataProvider *data) {
  if (m_data != data) {

    m_data = data;
    emit dataChanged();
  }
}

int Recitations::chapter() const {
  return m_chapter;
}

int Recitations::verse() const {
  return m_verse;
}

void Recitations::setChapter(int chapter) {
  if (chapter != m_chapter) {
    m_chapter = chapter;
    emit chapterChanged();
  }
}

void Recitations::setVerse(int verse) {
  if (m_verse != verse) {
    m_verse = verse;
    emit verseChanged();
  }
}

int Recitations::current() const {
  return m_current;
}

QVariantList Recitations::installed() const {
  QList<int> list = m_installed.keys();

  QVariantList ret;
  foreach (int r, list) {
    ret << r;
  }

  return ret;
}

void Recitations::refresh() {
  QDir dir(m_settings->recitationsDir());
  dir.mkpath(".");

  qDeleteAll(m_installed.values());
  m_installed.clear();

  QStringList entries(dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

  entries += dir.entryList(QStringList() << "*.zip", QDir::Files | QDir::NoDotAndDotDot);

  int x = 0;

  foreach (const QString& entry, entries) {
    Recitation *r = Recitation::create(entry, dir.filePath(entry));
    if (r) {
      m_installed.insert(x++, r);
    }
  }


  emit installedChanged();
}

QString Recitations::recitationName(int id) {
  if (m_installed.contains(id)) {
    return m_installed[id]->name();
  }

  return QString();
}

bool Recitations::load(int id) {
  if (m_installed.isEmpty() || m_installed.keys().indexOf(id) == -1) {
    return false;
  }

  if (!m_player) {
    m_player = new MediaPlayer(this);

    m_playlist = new MediaPlaylist(m_data, this);
    MediaPlaylist *list = m_player->playlist();
    if (list) {
      list->deleteLater();
    }

    m_player->setPlaylist(m_playlist);
  }

  stop();

  QObject::connect(m_player, SIGNAL(error()),
		   this, SLOT(playerError()));
  QObject::connect(m_player, SIGNAL(stateChanged()),
		   this, SLOT(playerStateChanged()));
  QObject::connect(m_player, SIGNAL(mediaChanged()),
		   this, SLOT(playerMediaChanged()));

  m_recitation = m_installed[id];

  m_playlist->setRecitation(m_recitation);

  m_current = id;

  m_settings->setDefaultRecitation(m_recitation->id());

  emit currentChanged();

  return true;
}

bool Recitations::loadDefault() {
  if (m_installed.isEmpty()) {
    return false;
  }

  QString id = m_settings->defaultRecitation();
  QList<int> keys = m_installed.keys();

  foreach (int val, keys) {
    if (m_installed[val]->id() == id) {
      return load(val);
    }
  }

  return load(keys[0]);
}

void Recitations::unload() {
  if (m_player) {
    m_player->stop();
  }

  delete m_playlist;
  m_playlist = 0;

  delete m_player;
  m_player = 0;

  m_recitation = 0;

  m_policy->release();
}

bool Recitations::isPlaying() const {
  return m_player ? m_player->isPlaying() : false;
}

void Recitations::playerStateChanged() {
  emit playingChanged();

  if (!m_player->isPlaying()) {
    stop();
  }
}

void Recitations::playerError() {
  stop();
  emit error(tr("Failed to play audio"));
}

void Recitations::playerMediaChanged() {
  Media *media = m_player->media();

  int chapter = media->chapter() - 1;
  int verse = media->verse() - 1;

  switch (m_playlist->mode()) {
  case MediaPlaylist::PlayVerse:
    emit positionChanged(chapter, verse);
    setChapter(chapter);
    setVerse(verse);
    break;

  case MediaPlaylist::PlayPage:
    // We are playing a basmala that is not on the first page.
    // Just unset the position.
    if (chapter == 0 && verse == 0 && m_playlist->page() != 0) {
      setChapter(-1);
      setVerse(-1);

      break;
    }

    emit positionChanged(chapter, verse);

    setChapter(chapter);
    setVerse(verse);

    break;

  case MediaPlaylist::PlayChapter:
    if (verse == 0 && chapter == 0 && m_playlist->chapter() != 0) {
      setChapter(-1);
      setVerse(-1);

      break;
    }

    emit positionChanged(chapter, verse);

    setChapter(chapter);
    setVerse(verse);

    break;

  case MediaPlaylist::PlayPart:
    if (verse == 0 && chapter == 0) {
      if (m_playlist->part() == 0) {
	// We have 2 basmalas in the first part
	if (m_playlist->media().indexOf(media) == 0) {
	  // First sura.
	  // Nothing.
	}
	else {
	  setChapter(-1);
	  setVerse(-1);
	  break;
	}
      }
      else {
	// Any other part. Don't set a position
	setChapter(-1);
	setVerse(-1);
	break;
      }
    }

    emit positionChanged(chapter, verse);

    setChapter(chapter);
    setVerse(verse);

    break;
  }
}

void Recitations::policyAcquired() {
  if (m_player && m_play) {
    m_player->play();
  }
}

void Recitations::stop() {
  m_play = false;

  if (!m_player || !m_recitation) {
    return;
  }

  m_player->stop();

  m_policy->release();

  setChapter(-1);
  setVerse(-1);
}

void Recitations::play(int chapter, int verse) {
  if (!m_player || !m_recitation) {
    return;
  }

  m_player->stop();
  m_playlist->playVerse(chapter, verse);

  m_play = true;
  m_policy->acquire();
}

void Recitations::playPage(int number) {
  if (!m_player || !m_recitation) {
    return;
  }

  m_player->stop();
  m_playlist->playPage(number);

  m_play = true;
  m_policy->acquire();
}

void Recitations::playChapter(int chapter) {
  if (!m_player || !m_recitation) {
    return;
  }

  m_player->stop();
  m_playlist->playChapter(chapter);

  m_play = true;
  m_policy->acquire();
}

void Recitations::playPart(int part) {
  if (!m_player || !m_recitation) {
    return;
  }

  m_player->stop();
  m_playlist->playPart(part);

  m_play = true;
  m_policy->acquire();
}

void Recitations::policyDenied() {
  policyLost();
  emit error(tr("Audio in use by another application"));
}

void Recitations::policyLost() {
  m_play = false;

  if (m_player) {
    m_player->stop();
  }
}
