/*
 * Copyright (c) 2019-2020 Mohammed Sameer <msameer@foolab.org>.
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

#include "mediaplayback.h"
#include "mediaservice.h"
#include "pageinfo.h"
#include "chapterinfo.h"
#include "partinfo.h"
#include "bookmarks.h"
#include "media.h"
#include "mediaplayerconfig.h"
#include "recitation.h"
#include <QQmlInfo>

MediaPlayback::MediaPlayback(QObject *parent) :
  QObject(parent),
  m_service(new MediaService(this)),
  m_recitation(0) {

  QObject::connect(m_service, SIGNAL(stateChanged()), this, SIGNAL(stateChanged()));
  QObject::connect(m_service, SIGNAL(positionChanged(int, int)), this, SIGNAL(positionChanged(int, int)));
  QObject::connect(m_service, SIGNAL(error()), this, SIGNAL(error()));
}

MediaPlayback::~MediaPlayback() {
  delete m_service;
  m_service = 0;
}

Quran::PlaybackState MediaPlayback::state() {
  return m_service->state();
}

void MediaPlayback::playPage(int page) {
  PageInfo info(page);
  playRange(info.firstChapter(), info.firstVerse(), info.lastChapter(), info.lastVerse());
}

void MediaPlayback::playVerse(int chapter, int verse) {
  playRange(chapter, verse, chapter, verse);
}

void MediaPlayback::playChapter(int chapter) {
  ChapterInfo info(chapter);
  playRange(chapter, 0, chapter, info.length() - 1);
}

void MediaPlayback::playPart(int part) {
  PartInfo info(part);
  PageInfo p1(info.firstPage());
  PageInfo p2(info.firstPage() + info.numberOfPages() - 1);
  playRange(p1.firstChapter(), p1.firstVerse(), p2.lastChapter(), p2.lastVerse());
}

void MediaPlayback::playRange(uint fromChapter, uint fromVerse, uint toChapter, uint toVerse) {
  if (!m_recitation) {
    qmlInfo(this) << "No recitation set";
    emit error();
    return;
  }

  if (fromChapter == toChapter) {
    if (fromVerse > toVerse) {
      emit error();
      return;
    }
  }

  if (fromChapter > toChapter) {
    emit error();
    return;
  }

  QList<Media> media;

  int index = 0;
  if (fromChapter == toChapter) {
    ChapterInfo info(fromChapter);
    for (int x = fromVerse; x <= toVerse; x++) {
      if (toVerse - fromVerse > 0) {
	// playing a sequence of a chapter:
	if (x == 0 && info.hasBasmala()) {
	  // Signal the basmala only if we are at chapter 0
	  media << Media(1, 1, index++, fromChapter == 0);
	}
      }
      media << Media(fromChapter + 1, x + 1, index++, true);
    }
  } else {
    for (int c = fromChapter; c <= toChapter; c++) {
      ChapterInfo info(c);
      if (c == fromChapter) {
	if (fromVerse == 0) {
	  if (info.hasBasmala()) {
	    // Signal the basmala only if we are at chapter 0
	    media << Media(1, 1, index++, c == 0);
	  }
	}

	for (int x = fromVerse; x < info.length(); x++) {
	  media << Media(c + 1, x + 1, index++, true);
	}
      } else if (c == toChapter) {
	// We must be starting a new chapter because fromChapter != toChapter
	if (info.hasBasmala()) {
	  // Signal the basmala only if we are at chapter 0
	  media << Media(1, 1, index++, c == 0);
	}
	for (int x = 0; x <= toVerse; x++) {
	  media << Media(c + 1, x + 1, index++, true);
	}
      } else {
	// Add all of it:
	if (info.hasBasmala()) {
	  // Signal the basmala only if we are at chapter 0
	  media << Media(1, 1, index++, c == 0);
	}
	for (int x = 0; x < info.length(); x++) {
	  media << Media(c + 1, x + 1, index++, true);
	}
      }
    }
  }

  MediaPlayerConfig conf;
  conf.setMedia(media);
  conf.setReciter(m_recitation->name());
  conf.setDownloadUrl(m_recitation->downloadUrl().toString());
  conf.setLocalPath(m_recitation->localPath());

  m_service->play(conf);
}

void MediaPlayback::stop() {
  return m_service->stop();
}

void MediaPlayback::pause() {
  return m_service->pause();
}

void MediaPlayback::resume() {
  m_service->resume();
}

Recitation *MediaPlayback::recitation() const {
  return m_recitation;
}

void MediaPlayback::setRecitation(Recitation *recitation) {
  m_recitation = recitation;
}
