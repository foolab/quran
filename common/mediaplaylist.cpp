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

#include "mediaplaylist.h"
#include "recitation.h"
#include <QDebug>
#include "dataprovider.h"
#include "media.h"

MediaPlaylist::MediaPlaylist(DataProvider *data, Recitation *recitation, QObject *parent)
  : QObject(parent),
    m_data(data),
    m_recitation(recitation),
    m_playingId(-1) {

}

MediaPlaylist::~MediaPlaylist() {

}

MediaPlaylist::PlayMode MediaPlaylist::mode() {
  return m_mode;
}

void MediaPlaylist::playPage(int page) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayPage;
  m_playingId = page;

  Page p = m_data->pageFromIndex(page);
  QList<Fragment> frags = p.fragments();

  int index = 0;

  foreach (const Fragment& f, frags) {
    if (f.start() == 0) {
      Sura s = m_data->sura(f.sura());

      if (s.hasBasmala()) {
	addMedia(m_recitation->mediaUrl(1, 1, index++));
      }
    }

    for (int x = f.start(); x < f.start() + f.size(); x++) {
      addMedia(m_recitation->mediaUrl(f.sura() + 1, x + 1, index++));
    }
  }
}

void MediaPlaylist::playChapter(int chapter) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayChapter;
  m_playingId = chapter;

  Sura s = m_data->sura(chapter);

  int index = 0;

  if (s.hasBasmala()) {
    addMedia(m_recitation->mediaUrl(1, 1, index++));
  }

  for (int x = 0; x < s.size(); x++) {
    addMedia(m_recitation->mediaUrl(chapter + 1, x + 1, index++));
  }
}

void MediaPlaylist::playVerse(int chapter, int verse) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayVerse;
  m_playingId = -1;

  addMedia(m_recitation->mediaUrl(chapter + 1, verse + 1, 0));
}

void MediaPlaylist::playPart(int part) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayPart;
  m_playingId = part;

  QList<Fragment> frags = m_data->fragmentsForPart(part);

  int index = 0;

  foreach (const Fragment& frag, frags) {
    if (frag.start() == 0) {
      Sura s = m_data->sura(frag.sura());

      if (s.hasBasmala()) {
	addMedia(m_recitation->mediaUrl(1, 1, index++));
      }
    }

    for (int x = frag.start(); x < frag.start() + frag.size(); x++) {
      addMedia(m_recitation->mediaUrl(frag.sura() + 1, x + 1, index++));
    }
  }
}

void MediaPlaylist::clear() {
  emit cleared();

  qDeleteAll(m_media);
  m_media.clear();
}

void MediaPlaylist::addMedia(Media *media) {
  m_media << media;

  emit mediaAdded(media);
}

Recitation *MediaPlaylist::recitation() {
  return m_recitation;
}

const QList<Media *> MediaPlaylist::media() const {
  return m_media;
}

bool MediaPlaylist::signalMedia(int index, int& chapter, int& verse) const {
  const Media *media = m_media[index];

  chapter = media->chapter() - 1;
  verse = media->verse() - 1;

  switch (m_mode) {
  case MediaPlaylist::PlayVerse:
    return true;

  case MediaPlaylist::PlayPage:
  case MediaPlaylist::PlayChapter:
    if (chapter == 0 && verse == 0 && m_playingId != 0) {
      // We are playing a basmala that is not on the first page or first chapter.
      // Just unset the position.
      return false;
    }

    return true;

  case MediaPlaylist::PlayPart:
    if (verse == 0 && chapter == 0) {
      // We are reciting a basmala
      if (m_playingId == 0) {
	// We have 2 basmalas in the first part
	if (m_media.first() == media) {
	  // First sura has a basmala
	  return true;
	}
	else {
	  return false;
	}
      }
      else {
	// Any other part. Don't set a position
	return false;
      }
    }

    return true;
  }
}
