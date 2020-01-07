/*
 * Copyright (c) 2020 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef MEDIA_STATE_H
#define MEDIA_STATE_H

#include <QObject>

class MediaPlayerConfig;
class QSettings;

class MediaState : public QObject {
  Q_OBJECT

public:
  MediaState(QObject *parent = 0);
  ~MediaState();

  void setPosition(int position);
  int position();

  void setConfig(const MediaPlayerConfig& config);
  MediaPlayerConfig config();

  void clear();

public slots:
  void save();

private:
  QSettings *m_settings;
  bool m_hasChanges;
};

#endif /* MEDIA_STATE_H */
