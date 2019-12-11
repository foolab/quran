/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#include "mediaplayerconfig.h"

MediaPlayerConfig::MediaPlayerConfig() {

}

MediaPlayerConfig::~MediaPlayerConfig() {

}

QString MediaPlayerConfig::localPath() const {
  return m_localPath;
}

void MediaPlayerConfig::setLocalPath(const QString& localPath) {
  m_localPath = localPath;
}

QString MediaPlayerConfig::downloadUrl() const {
  return m_downloadUrl;
}

void MediaPlayerConfig::setDownloadUrl(const QString& downloadUrl) {
  m_downloadUrl = downloadUrl;
}

QString MediaPlayerConfig::reciter() const {
  return m_reciter;
}

void MediaPlayerConfig::setReciter(const QString& reciter) {
  m_reciter = reciter;
}

QList<Media> MediaPlayerConfig::media() const {
  return m_media;
}

void MediaPlayerConfig::setMedia(const QList<Media>& media) {
  m_media = media;
}

