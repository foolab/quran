/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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
#include "media.h"
#include "downloader.h"
#include <QNetworkReply>
#include "bookmarks.h"
#include <QQmlInfo>
#include "chapterinfo.h"
#include "pageinfo.h"
#include "partinfo.h"

MediaPlaylist::MediaPlaylist(Recitation *recitation,
			     Downloader *downloader, QObject *parent) :
  QObject(parent),
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

  QList<FragmentInfo> frags = PageInfo(page).fragments();

  int index = 0;

  for (const FragmentInfo& f : frags) {
    if (f.start() == 0) {
      if (ChapterInfo(f.chapter()).hasBasmala()) {
	addMedia(Media(m_recitation, 1, 1, index++));
      }
    }

    for (int x = f.start(); x < f.start() + f.length(); x++) {
      addMedia(Media(m_recitation, f.chapter() + 1, x + 1, index++));
    }
  }
}

void MediaPlaylist::playChapter(int chapter) {
  if (!m_recitation) {
    return;
  }

  m_mode = PlayChapter;
  m_playingId = chapter;

  int index = 0;

  ChapterInfo info(chapter);
  if (info.hasBasmala()) {
    addMedia(Media(m_recitation, 1, 1, index++));
  }

  for (int x = 0; x < info.length(); x++) {
    addMedia(Media(m_recitation, chapter + 1, x + 1, index++));
  }
}

void MediaPlaylist::playVerse(int chapter, int verse) {
  if (!m_recitation) {
    return;
  }

  m_mode = PlayVerse;
  m_playingId = -1;

  addMedia(Media(m_recitation, chapter + 1, verse + 1, 0));
}

void MediaPlaylist::playPart(int part) {
  if (!m_recitation) {
    return;
  }

  m_mode = PlayPart;
  m_playingId = part;

  QList<FragmentInfo> frags;
  PartInfo p(part);

  for (int x = p.firstPage(); x < p.firstPage() + p.numberOfPages(); x++) {
    frags.append(PageInfo(x).fragments());
  }

  int index = 0;

  for (const FragmentInfo& frag : frags) {
    if (frag.start() == 0) {
      if (ChapterInfo(frag.chapter()).hasBasmala()) {
	addMedia(Media(m_recitation, 1, 1, index++));
      }
    }

    for (int x = frag.start(); x < frag.start() + frag.length(); x++) {
      addMedia(Media(m_recitation, frag.chapter() + 1, x + 1, index++));
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

  // Silence g++
  return false;
}

void MediaPlaylist::start() {
  if (m_recitation->type() == Recitation::Online) {
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

MediaPlaylist *MediaPlaylist::partList(Recitation *recitation,
				       Downloader *downloader, uint part, QObject *parent) {

  MediaPlaylist *list = new MediaPlaylist(recitation, downloader, parent);

  list->playPart(part);

  return list;
}

MediaPlaylist *MediaPlaylist::pageList(Recitation *recitation,
				       Downloader *downloader, uint page, QObject *parent) {

  MediaPlaylist *list = new MediaPlaylist(recitation, downloader, parent);

  list->playPage(page);

  return list;
}

MediaPlaylist *MediaPlaylist::verseList(Recitation *recitation,
					Downloader *downloader,
					uint serialized, QObject *parent) {

  int chapter, verse;
  Bookmarks::deserialize(serialized, chapter, verse);

  MediaPlaylist *list = new MediaPlaylist(recitation, downloader, parent);

  list->playVerse(chapter, verse);

  return list;
}

MediaPlaylist *MediaPlaylist::chapterList(Recitation *recitation,
					  Downloader *downloader, uint chapter, QObject *parent) {

  MediaPlaylist *list = new MediaPlaylist(recitation, downloader, parent);

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

  m_reply = m_downloader->get(m_recitation->downloadUrl(m));

  QObject::connect(m_reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void MediaPlaylist::replyFinished() {
  QByteArray data = m_reply->readAll();

  if (m_reply->error() != QNetworkReply::NoError || data.isEmpty()) {
    if (m_reply->error() != QNetworkReply::NoError) {
      qmlInfo(this) << "Error downloading: " << m_reply->url() << " : " << m_reply->errorString();
    }

    m_queue.clear();
    m_reply->deleteLater();
    m_reply = 0;
    emit mediaAvailable(Media::error());
    return;
  }

  const Media& media = m_queue.dequeue();
  if (!media.setData(data)) {
    qmlInfo(this) << "Failed to write data for " << m_reply->url();

    m_queue.clear();
    m_reply->deleteLater();
    m_reply = 0;

    emit mediaAvailable(Media::error());
    return;
  }

  m_reply->deleteLater();
  m_reply = 0;

  emit mediaAvailable(media);

  download();
}
