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
#include "downloader.h"
#include <QNetworkReply>

MediaPlaylist::MediaPlaylist(DataProvider *data, Recitation *recitation,
			     Downloader *downloader, QObject *parent)
  : QObject(parent),
    m_data(data),
    m_recitation(recitation),
    m_downloader(downloader),
    m_playingId(-1),
    m_reply(0) {

}

MediaPlaylist::~MediaPlaylist() {
  m_media.clear();
}

void MediaPlaylist::playPage(int page) {
  if (!m_recitation) {
    return;
  }

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

  m_mode = PlayVerse;
  m_playingId = -1;

  addMedia(m_recitation->mediaUrl(chapter + 1, verse + 1, 0));
}

void MediaPlaylist::playPart(int part) {
  if (!m_recitation) {
    return;
  }

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

Recitation *MediaPlaylist::recitation() {
  return m_recitation;
}

const QList<Media> MediaPlaylist::media() const {
  return m_media;
}

bool MediaPlaylist::signalMedia(int index, int& chapter, int& verse) const {
  const Media& media = m_media[index];

  chapter = media.chapter() - 1;
  verse = media.verse() - 1;

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
	if (m_media.first().index() == media.index()) {
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

void MediaPlaylist::start() {
  if (m_recitation->isOnline()) {
    download();
  }
  else {
    foreach (const Media& media, m_media) {
      emit mediaAvailable(media);
    }

    emit mediaAvailable(Media::eos());
  }
}

void MediaPlaylist::stop() {
  if (m_reply) {
    delete m_reply;
    m_reply = 0;
  }

  m_queue.clear();
  m_media.clear();
}

void MediaPlaylist::addMedia(const Media& media) {
  m_media << media;
  m_queue.enqueue(media);
}

MediaPlaylist *MediaPlaylist::partList(DataProvider *data, Recitation *recitation,
				       Downloader *downloader, int part, QObject *parent) {

  MediaPlaylist *list = new MediaPlaylist(data, recitation, downloader, parent);

  list->playPart(part);

  return list;
}

MediaPlaylist *MediaPlaylist::pageList(DataProvider *data, Recitation *recitation,
				       Downloader *downloader, int page, QObject *parent) {

  MediaPlaylist *list = new MediaPlaylist(data, recitation, downloader, parent);

  list->playPage(page);

  return list;
}

MediaPlaylist *MediaPlaylist::verseList(DataProvider *data, Recitation *recitation,
					Downloader *downloader,
					int chapter, int verse, QObject *parent) {

  MediaPlaylist *list = new MediaPlaylist(data, recitation, downloader, parent);

  list->playVerse(chapter, verse);

  return list;
}

MediaPlaylist *MediaPlaylist::chapterList(DataProvider *data, Recitation *recitation,
					  Downloader *downloader, int chapter, QObject *parent) {

  MediaPlaylist *list = new MediaPlaylist(data, recitation, downloader, parent);

  list->playChapter(chapter);

  return list;
}

void MediaPlaylist::download() {
  if (m_queue.isEmpty()) {
    emit mediaAvailable(Media::eos());
    return;
  }

  // NOTE: We can not use a reference here. otherwise the signal argument will contain garbage.
  const Media m = m_queue.head();
  QByteArray data = m.data();
  if (!data.isEmpty()) {
    m_queue.dequeue();
    emit mediaAvailable(m);
    download();
    return;
  }

  m_reply = m_downloader->get(m.alternateUrl());

  QObject::connect(m_reply, SIGNAL(sslErrors(const QList<QSslError>&)), m_reply,
		   SLOT(ignoreSslErrors()));
  QObject::connect(m_reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void MediaPlaylist::replyFinished() {
  QByteArray data = m_reply->readAll();

  if (m_reply->error() != QNetworkReply::NoError || data.isEmpty()) {
    m_queue.clear();
    m_reply->deleteLater();
    m_reply = 0;
    emit mediaAvailable(Media::error());
    return;
  }

  const Media& media = m_queue.dequeue();
  if (!media.setData(data)) {
    m_queue.clear();
    emit mediaAvailable(Media::error());
    return;
  }

  emit mediaAvailable(media);

  download();
}
