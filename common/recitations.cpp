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
#include "mediaplaylist.h"

Recitations::Recitations(QObject *parent)
  : QObject(parent),
    m_settings(0),
    m_data(0),
    m_player(0),
    m_recitation(0),
    m_current(0),
    m_chapter(-1),
    m_verse(-1),
    m_playingId(-1) {

}

Recitations::~Recitations() {
  if (m_player) {
    m_player->stop();
  }

  qDeleteAll(m_installed.values());
  m_installed.clear();

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

  stop();

  m_recitation = m_installed[id];

  m_current = id;

  m_settings->setDefaultRecitation(m_recitation->id());

  if (!m_player) {
    m_player = new MediaPlayer(this);
  }

  MediaPlaylist *list = new MediaPlaylist(m_data, m_recitation, this);
  m_player->setPlaylist(list);

  QObject::connect(m_player, SIGNAL(error()),
		   this, SLOT(playerError()));
  QObject::connect(m_player, SIGNAL(stateChanged()),
		   this, SLOT(playerStateChanged()));

  QObject::connect(m_player, SIGNAL(positionChanged(int, int, int)),
		   this, SLOT(playerPositionChanged(int, int, int)));

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
  stop();

  if (m_player) {
    delete m_player;
    m_player = 0;
  }

  m_recitation = 0;
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

void Recitations::playerPositionChanged(int chapter, int verse, int index) {
  --chapter;
  --verse;

  switch (m_player->playlist()->mode()) {
  case MediaPlaylist::PlayVerse:
    emit positionChanged(chapter, verse);
    setChapter(chapter);
    setVerse(verse);
    break;

  case MediaPlaylist::PlayPage:
    // We are playing a basmala that is not on the first page.
    // Just unset the position.
    if (chapter == 0 && verse == 0 && m_playingId != 0) {
      setChapter(-1);
      setVerse(-1);

      break;
    }

    emit positionChanged(chapter, verse);

    setChapter(chapter);
    setVerse(verse);

    break;

  case MediaPlaylist::PlayChapter:
    if (verse == 0 && chapter == 0 && m_playingId != 0) {
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
      if (m_playingId == 0) {
	// We have 2 basmalas in the first part
	if (index == 0) {
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

void Recitations::stop() {
  if (m_player) {
    m_player->stop();
  }

  setChapter(-1);
  setVerse(-1);
}

void Recitations::play(int chapter, int verse) {
  if (!m_player || !m_recitation) {
    return;
  }

  stop();

  m_playingId = -1;

  m_player->playlist()->playVerse(chapter, verse);
  m_player->play();
}

void Recitations::playPage(int number) {
  if (!m_player || !m_recitation) {
    return;
  }

  stop();

  m_playingId = number;

  m_player->playlist()->playPage(number);
  m_player->play();
}

void Recitations::playChapter(int chapter) {
  if (!m_player || !m_recitation) {
    return;
  }

  stop();

  m_playingId = chapter;

  m_player->playlist()->playChapter(chapter);
  m_player->play();
}

void Recitations::playPart(int part) {
  if (!m_player || !m_recitation) {
    return;
  }

  stop();

  m_playingId = part;

  m_player->playlist()->playPart(part);
  m_player->play();
}
