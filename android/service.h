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

#ifndef SERVICE_H
#define SERVICE_H

#include <QAndroidService>
#include <QAndroidBinder>

#define SERVICE            "org.foolab.quran.MediaService"

#define ACTION_PLAY        "play"
#define ACTION_STOP        "stop"
#define ACTION_PAUSE       "pause"
#define ACTION_RESUME      "resume"

#define FLIP_TO_PAUSE      "flip-to-pause"

#define KEY_CONFIG         "conf"
#define KEY_RECITER        "reciter"

class Binder;
class MediaPlayer;
class Intent;
class FlipSensor;

class Service : public QAndroidService {
  Q_OBJECT

public:
  Service(int& argc, char **argv);
  ~Service();

  QAndroidBinder *onBind(const QAndroidIntent& intent);

  typedef enum {
    UpdateBinder = 1,
    ActionUpdatePosition,
    ActionStateChanged,
    ActionError,
    QueryPosition,
    QueryState,
    UpdateSettings,
  } Action;

  bool onStartCommand(Intent& intent);

private slots:
  void stateChanged();
  void positionChanged(int chapter, int verse);
  void error();
  uint getPosition();
  void sendState();
  bool updateSettings(const QAndroidParcel& data);
  void pause();

private:
  void stopService();
  void send(int code, const QVariant& data);

  void setSensorState();

  QAndroidBinder m_sender;
  MediaPlayer *m_player;
  Binder *m_localBinder;
  FlipSensor *m_sensor;
  int m_chapter;
  int m_verse;
  bool m_flipToPause;
};

#endif /* SERVICE_H */
