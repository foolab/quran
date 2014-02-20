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
    m_downloader(0),
    m_current(0),
    m_chapter(-1),
    m_verse(-1) {

}

Recitations::~Recitations() {
  if (m_player) {
    m_player->stop();
  }

  qDeleteAll(m_installed.values());
  m_installed.clear();

  if (m_player) {
    delete m_player;
    m_player = 0;
  }

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


Downloader *Recitations::downloader() const {
  return m_downloader;
}

void Recitations::setDownloader(Downloader *downloader) {
  if (m_downloader != downloader) {

    m_downloader = downloader;
    emit downloaderChanged();
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

QList<int> Recitations::installed() const {
  return m_installed.keys();
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

  emit refreshed();
  emit installedCountChanged();
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

  QObject::connect(m_player, SIGNAL(error()),
		   this, SLOT(playerError()));
  QObject::connect(m_player, SIGNAL(stateChanged()),
		   this, SLOT(playerStateChanged()));

  QObject::connect(m_player, SIGNAL(positionChanged(int, int)),
		   this, SLOT(playerPositionChanged(int, int)));

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
  if (!m_player->isPlaying()) {
    stop();
  }

  emit playingChanged();
}

void Recitations::playerError() {
  stop();
  emit error(tr("Failed to play audio"));
}

void Recitations::playerPositionChanged(int chapter, int verse) {
  setChapter(chapter);
  setVerse(verse);
  emit positionChanged(chapter, verse);
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

  m_player->start(MediaPlaylist::verseList(m_data, m_recitation, m_downloader,
					   chapter, verse, m_player));
}

void Recitations::playPage(int page) {
  if (!m_player || !m_recitation) {
    return;
  }

  stop();

  m_player->start(MediaPlaylist::pageList(m_data, m_recitation, m_downloader, page, m_player));
}

void Recitations::playChapter(int chapter) {
  if (!m_player || !m_recitation) {
    return;
  }

  stop();

  m_player->start(MediaPlaylist::chapterList(m_data, m_recitation, m_downloader,
					     chapter, m_player));
}

void Recitations::playPart(int part) {
  if (!m_player || !m_recitation) {
    return;
  }

  stop();

  m_player->start(MediaPlaylist::partList(m_data, m_recitation, m_downloader, part, m_player));
}

int Recitations::installedCount() const {
  return m_installed.size();
}
