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
#include "mediaplayerconfig.h"
#include "media.h"
#include "downloader.h"
#include <QNetworkReply>
#include <QQmlInfo>
#include "recitationdataprovider.h"

MediaPlaylist::MediaPlaylist(const MediaPlayerConfig& config, QObject *parent) :
  QObject(parent),
  m_downloader(new Downloader(this)),
  m_url(config.downloadUrl()),
  m_download(0) {

  m_dataProvider = new RecitationDataProvider(config.localPath());

  for (const Media& media : config.media()) {
    addMedia(media);
  }
}

MediaPlaylist::~MediaPlaylist() {
  m_media.clear();

  delete m_dataProvider;
  m_dataProvider = 0;
}

RecitationDataProvider *MediaPlaylist::dataProvider() const {
  return m_dataProvider;
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
  if (!m_dataProvider->isLocal()) {
    download();
  } else {
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

void MediaPlaylist::download() {
  if (m_queue.isEmpty()) {
    emit mediaAvailable(Media::eos());
    return;
  }

  // NOTE: We can not use a reference here. otherwise the signal argument will contain garbage.
  const Media m = m_queue.head();
  QByteArray data = m_dataProvider->data(m);
  if (!data.isEmpty()) {
    m_queue.dequeue();
    emit mediaAvailable(m);
    download();
    return;
  }

  QUrl url(QString("%1/%2%3.mp3")
	   .arg(m_url)
	   .arg(m.chapter(), 3, 10, QChar('0'))
	   .arg(m.verse(), 3, 10, QChar('0')));

  m_download = m_downloader->get(url);

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
  if (!m_dataProvider->setData(media, data)) {
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
