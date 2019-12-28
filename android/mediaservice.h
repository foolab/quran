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

#ifndef MEDIA_SERVICE_H
#define MEDIA_SERVICE_H

#include <QObject>

class ServiceConnection;
class MediaPlayerConfig;
class Binder;

class MediaService : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged);
  Q_PROPERTY(bool paused READ isPaused NOTIFY pausedChanged);

public:
  MediaService(QObject *parent = 0);
  ~MediaService();

  void play(const MediaPlayerConfig& config);

  bool isPlaying();
  bool isPaused();

public slots:
  void stop();
  void pause();
  void resume();

signals:
  void playingChanged();
  void pausedChanged();
  void positionChanged(int chapter, int verse);
  void error();

private:
  bool send(int code, const QByteArray *data = 0);
  bool get(int code);

  Binder *m_binder;
  ServiceConnection *m_connection;
};

#endif /* MEDIA_SERVICE_H */
