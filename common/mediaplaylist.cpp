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

MediaPlaylist::MediaPlaylist(DataProvider *data, QObject *parent)
  : QObject(parent), m_data(data), m_recitation(0),
    m_chapter(0), m_page(0), m_part(0) {

}

MediaPlaylist::~MediaPlaylist() {

}

void MediaPlaylist::setRecitation(Recitation *recitation) {
  m_recitation = recitation;
}

MediaPlaylist::PlayMode MediaPlaylist::mode() {
  return m_mode;
}

int MediaPlaylist::page() {
  return m_page;
}

int MediaPlaylist::chapter() {
  return m_chapter;
}

int MediaPlaylist::part() {
  return m_part;
}

void MediaPlaylist::playPage(int page) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayPage;
  m_page = page;

  Page p = m_data->pageFromIndex(page);
  QList<Fragment> frags = p.fragments();

  foreach (const Fragment& f, frags) {
    if (f.start() == 0) {
      Sura s = m_data->sura(f.sura());

      if (s.hasBasmala()) {
	addMedia(m_recitation->mediaUrl(1, 1));
      }
    }

    for (int x = f.start(); x < f.start() + f.size(); x++) {
      addMedia(m_recitation->mediaUrl(f.sura() + 1, x + 1));
    }
  }
}

void MediaPlaylist::playChapter(int chapter) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayChapter;
  m_chapter = chapter;

  Sura s = m_data->sura(chapter);

  if (s.hasBasmala()) {
    addMedia(m_recitation->mediaUrl(1, 1));
  }

  for (int x = 0; x < s.size(); x++) {
    addMedia(m_recitation->mediaUrl(chapter + 1, x + 1));
  }
}

void MediaPlaylist::playVerse(int chapter, int verse) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_mode = PlayVerse;

  addMedia(m_recitation->mediaUrl(chapter + 1, verse + 1));
}

void MediaPlaylist::playPart(int part) {
  if (!m_recitation) {
    return;
  }

  clear();

  m_part = part;

  m_mode = PlayPart;

  QList<Fragment> frags = m_data->fragmentsForPart(part);

  foreach (const Fragment& frag, frags) {
    if (frag.start() == 0) {
      Sura s = m_data->sura(frag.sura());

      if (s.hasBasmala()) {
	addMedia(m_recitation->mediaUrl(1, 1));
      }
    }

    for (int x = frag.start(); x < frag.start() + frag.size(); x++) {
      addMedia(m_recitation->mediaUrl(frag.sura() + 1, x + 1));
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

QList<Media *> MediaPlaylist::media() {
  return m_media;
}

Recitation *MediaPlaylist::recitation() {
  return m_recitation;
}
