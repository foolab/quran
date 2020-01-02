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

#ifndef MEDIA_PLAYBACK_H
#define MEDIA_PLAYBACK_H

#include <QObject>

class MediaService;
class Recitation;

class MediaPlayback : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged);
  Q_PROPERTY(bool paused READ isPaused NOTIFY pausedChanged);

public:
  MediaPlayback(QObject *parent = 0);
  ~MediaPlayback();

  bool isPlaying() const;
  bool isPaused() const;

  Recitation *recitation() const;
  void setRecitation(Recitation *recitation);

public slots:
  void playPage(int page);
  void playVerse(int chapter, int verse);
  void playChapter(int chapter);
  void playPart(int part);
  void playRange(uint fromChapter, uint fromVerse, uint toChapter, uint toVerse);

  void stop();
  void pause();
  void resume();

signals:
  void playingChanged();
  void pausedChanged();
  void positionChanged(int chapter, int verse);
  void error();

private:
  MediaService *m_service;
  Recitation *m_recitation;
};

#endif /* MEDIA_PLAYBACK_H */
