/*
 * Copyright (c) 2011-2013 Mohammed Sameer <msameer@foolab.org>.
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
#include <QMediaPlayer>
#include "audiopolicy.h"
#include "mediaplaylist.h"
#include "phoneflipcontrol.h"

Recitations::Recitations(const QString& dir, Downloader *downloader, Settings *settings,
			 DataProvider *data, QObject *parent)
  : QObject(parent), m_downloader(downloader), m_dir(dir), m_settings(settings),
    m_data(data), m_player(0), m_playlist(0), m_recitation(0), m_current(0),
    m_policy(new AudioPolicy(this)), m_play(false),
    m_chapter(-1), m_verse(-1), m_flipControl(new PhoneFlipControl(m_settings, this)) {

  QObject::connect(m_policy, SIGNAL(acquired()), this, SLOT(policyAcquired()));
  QObject::connect(m_policy, SIGNAL(lost()), this, SLOT(policyLost()));
  QObject::connect(m_policy, SIGNAL(denied()), this, SLOT(policyDenied()));
  QObject::connect(m_flipControl, SIGNAL(flipped()), this, SLOT(stop()));
}

Recitations::~Recitations() {
  qDeleteAll(m_installed.values());
  m_installed.clear();

  m_flipControl->stop();

  if (m_player) {
    m_player->stop();
  }

  delete m_playlist;
  m_playlist = 0;

  delete m_player;
  m_player = 0;

  m_recitation = 0;
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

QList<int> Recitations::installed() const {
  return m_installed.keys();
}

void Recitations::refresh() {
  m_dir.mkpath(".");

  qDeleteAll(m_installed.values());
  m_installed.clear();

  QStringList entries(m_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

  entries += m_dir.entryList(QStringList() << "*.zip", QDir::Files | QDir::NoDotAndDotDot);

  int x = 0;

  foreach (const QString& entry, entries) {
    Recitation *r = Recitation::create(entry, m_dir.filePath(entry));
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
    m_player = new QMediaPlayer(this);

    m_playlist = new MediaPlaylist(m_settings, m_data, this);
    QMediaPlaylist *list = m_player->playlist();
    if (list) {
      list->deleteLater();
    }

    m_player->setPlaylist(m_playlist);
  }

  stop();

  QObject::connect(m_player, SIGNAL(error(QMediaPlayer::Error)),
		   this, SLOT(playerError()));
  QObject::connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
		   this, SLOT(playerStateChanged()));
  QObject::connect(m_player, SIGNAL(mediaChanged(const QMediaContent&)),
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

  m_flipControl->stop();
}

bool Recitations::isPlaying() const {
  return m_player ? m_player->state() != QMediaPlayer::StoppedState : false;
}

void Recitations::playerStateChanged() {
  emit playingChanged();

  if (m_player->state() != QMediaPlayer::PlayingState) {
    stop();
  }
}

void Recitations::playerError() {
  m_play = false;
  m_flipControl->stop();
  m_player->stop();
  emit error(m_player->errorString());

  setChapter(-1);
  setVerse(-1);
}

void Recitations::playerMediaChanged() {
  QString id = QFileInfo(m_player->media().canonicalUrl().toString()).fileName();

  if (id.isEmpty()) {
    return;
  }

  int chapter = id.left(3).toInt() - 1;
  int verse = id.mid(3, 3).toInt() - 1;

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
	if (m_playlist->currentIndex() == 0) {
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
    m_flipControl->start();
    m_player->play();
  }
}

void Recitations::stop() {
  m_play = false;

  if (!m_player || !m_recitation) {
    return;
  }

  m_flipControl->stop();
  m_player->stop();

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
    m_flipControl->stop();
    m_player->stop();
  }
}
