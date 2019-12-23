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

#ifndef SERVICE_H
#define SERVICE_H

#include <QAndroidService>

class Binder;
class MediaPlayer;

class Service : public QAndroidService {
  Q_OBJECT

public:
  Service(int& argc, char **argv);
  ~Service();

  QAndroidBinder *onBind(const QAndroidIntent& intent);

  typedef enum {
    UpdateBinder = 0,
    ActionPlay = 1,
    ActionStop = 2,
    ActionPause = 3,
    ActionResume = 4,
    ActionUpdatePosition = 5,
    ActionPlayingChanged = 6,
    ActionPausedChanged = 7,
    ActionError = 8,
    QueryPosition = 9,
    QueryPaused = 10,
    QueryPlaying = 11,
  } Action;

private slots:
  void playingChanged();
  void pausedChanged();
  void positionChanged(int chapter, int verse);
  void error();
  uint getPosition();

private:
  void send(int code, const QVariant& data);

  MediaPlayer *m_player;
  Binder *m_receiver;
  int m_chapter;
  int m_verse;
};

#endif /* SERVICE_H */
