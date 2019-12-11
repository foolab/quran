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

#ifndef MEDIA_PLAYER_CONFIG_H
#define MEDIA_PLAYER_CONFIG_H

#include <QString>
#include <QList>
#include "media.h"

class MediaPlayerConfig {
public:
  MediaPlayerConfig();
  ~MediaPlayerConfig();

  QString localPath() const;
  void setLocalPath(const QString& localPath);

  QString downloadUrl() const;
  void setDownloadUrl(const QString& downloadUrl);

  QString reciter() const;
  void setReciter(const QString& reciter);

  QList<Media> media() const;
  void setMedia(const QList<Media>& media);

private:
  QString m_localPath;
  QString m_downloadUrl;
  QString m_reciter;
  QList<Media> m_media;
};

#endif /* MEDIA_PLAYER_CONFIG_H */
