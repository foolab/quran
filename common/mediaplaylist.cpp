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
  m_download(0) {

}

MediaPlaylist::~MediaPlaylist() {
  m_media.clear();
}

void MediaPlaylist::playPage(int page) {
  if (!m_recitation) {
    return;
  }

  QList<FragmentInfo> frags = PageInfo(page).fragments();

  int index = 0;

  for (const FragmentInfo& f : frags) {
    if (f.start() == 0) {
      if (ChapterInfo(f.chapter()).hasBasmala()) {
	// Signal the basmala only if we are at chapter 0
	addMedia(Media(m_recitation, 1, 1, index++, f.chapter() == 0));
      }
    }

    for (int x = f.start(); x < f.start() + f.length(); x++) {
      addMedia(Media(m_recitation, f.chapter() + 1, x + 1, index++, true));
    }
  }
}

void MediaPlaylist::playRange(uint fromChapter, uint fromVerse, uint toChapter, uint toVerse) {
  if (!m_recitation) {
    return;
  }

  int index = 0;
  if (fromChapter == toChapter) {
    ChapterInfo info(fromChapter);
    for (int x = fromVerse; x <= toVerse; x++) {
      addMedia(Media(m_recitation, fromChapter + 1, x + 1, index++, true));
    }
  } else {
    for (int c = fromChapter; c <= toChapter; c++) {
      ChapterInfo info(c);
      if (c == fromChapter) {
	if (fromVerse == 0) {
	  if (info.hasBasmala()) {
	    // Signal the basmala only if we are at chapter 0
	    addMedia(Media(m_recitation, 1, 1, index++, c == 0));
	  }
	}

	for (int x = fromVerse; x < info.length(); x++) {
	  addMedia(Media(m_recitation, c + 1, x + 1, index++, true));
	}
      } else if (c == toChapter) {
	// We must be starting a new chapter because fromChapter != toChapter
	if (info.hasBasmala()) {
	  // Signal the basmala only if we are at chapter 0
	  addMedia(Media(m_recitation, 1, 1, index++, c == 0));
	}
	for (int x = 0; x <= toVerse; x++) {
	  addMedia(Media(m_recitation, c + 1, x + 1, index++, true));
	}
      } else {
	// Add all of it:
	if (info.hasBasmala()) {
	  // Signal the basmala only if we are at chapter 0
	  addMedia(Media(m_recitation, 1, 1, index++, c == 0));
	}
	for (int x = 0; x < info.length(); x++) {
	  addMedia(Media(m_recitation, c + 1, x + 1, index++, true));
	}
      }
    }
  }
}

void MediaPlaylist::playChapter(int chapter) {
  if (!m_recitation) {
    return;
  }

  int index = 0;

  ChapterInfo info(chapter);
  if (info.hasBasmala()) {
    // Signal the basmala only if we are at chapter 0
    addMedia(Media(m_recitation, 1, 1, index++, chapter == 0));
  }

  for (int x = 0; x < info.length(); x++) {
    addMedia(Media(m_recitation, chapter + 1, x + 1, index++, true));
  }
}

void MediaPlaylist::playVerse(int chapter, int verse) {
  if (!m_recitation) {
    return;
  }

  addMedia(Media(m_recitation, chapter + 1, verse + 1, 0, true));
}

void MediaPlaylist::playPart(int part) {
  if (!m_recitation) {
    return;
  }

  QList<FragmentInfo> frags;
  PartInfo p(part);

  for (int x = p.firstPage(); x < p.firstPage() + p.numberOfPages(); x++) {
    frags.append(PageInfo(x).fragments());
  }

  int index = 0;

  for (const FragmentInfo& frag : frags) {
    if (frag.start() == 0) {
      if (ChapterInfo(frag.chapter()).hasBasmala()) {
	// Signal the basmala only if we are at chapter 0
	addMedia(Media(m_recitation, 1, 1, index++, frag.chapter() == 0));
      }
    }

    for (int x = frag.start(); x < frag.start() + frag.length(); x++) {
      addMedia(Media(m_recitation, frag.chapter() + 1, x + 1, index++, true));
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

  return media.signal();
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
  if (m_download) {
    delete m_download;
    m_download = 0;
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

MediaPlaylist *MediaPlaylist::rangeList(Recitation *recitation,
					       Downloader *downloader,
					       uint fromChapter, uint fromVerse,
					       uint toChapter, uint toVerse,
					       QObject *parent) {
  MediaPlaylist *list = new MediaPlaylist(recitation, downloader, parent);

  list->playRange(fromChapter, fromVerse, toChapter, toVerse);

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

  m_download = m_downloader->get(m_recitation->downloadUrl(m));

  QObject::connect(m_download, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void MediaPlaylist::replyFinished() {
  QByteArray data = m_download->reply()->readAll();
  bool error = m_download->reply()->error() != QNetworkReply::NoError;

  if (error || data.isEmpty()) {
    if (error) {
      qmlInfo(this) << "Error downloading: "
		    << m_download->reply()->url()
		    << " : "
		    << m_download->reply()->errorString();
    }

    m_queue.clear();
    m_download->deleteLater();
    m_download = 0;
    emit mediaAvailable(Media::error());
    return;
  }

  const Media& media = m_queue.dequeue();
  if (!media.setData(data)) {
    qmlInfo(this) << "Failed to write data for "
		  << m_download->reply()->url();

    m_queue.clear();
    m_download->deleteLater();
    m_download = 0;

    emit mediaAvailable(Media::error());
    return;
  }

  m_download->deleteLater();
  m_download = 0;

  emit mediaAvailable(media);

  download();
}
