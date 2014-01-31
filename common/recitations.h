/*
 * Copyright (c) 2011-2014 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef RECITATIONS_H
#define RECITATIONS_H

#include <QObject>
#include <QDir>
#include <QMap>
#include <QVariant>

class Settings;
class DataProvider;
class Recitation;
class QMediaPlayer;
class AudioPolicy;
class MediaPlaylist;

class Recitations : public QObject {
  Q_OBJECT

  Q_PROPERTY(QVariantList installed READ installed NOTIFY installedChanged);
  Q_PROPERTY(int current READ current NOTIFY currentChanged);
  Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playingChanged);
  Q_PROPERTY(int chapter READ chapter NOTIFY chapterChanged);
  Q_PROPERTY(int verse READ verse NOTIFY verseChanged);
  Q_PROPERTY(Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged);
  Q_PROPERTY(DataProvider *data READ data WRITE setData NOTIFY dataChanged);

public:
  Recitations(QObject *parent = 0);
  ~Recitations();

  QVariantList installed() const;

  Q_INVOKABLE QString recitationName(int id);

  Q_INVOKABLE bool load(int id);
  Q_INVOKABLE bool loadDefault();

  int current() const;

  bool isPlaying() const;

  int chapter() const;
  int verse() const;

  Settings *settings() const;
  void setSettings(Settings *settings);

  DataProvider *data() const;
  void setData(DataProvider *data);

public slots:
  void refresh();
  void play(int chapter, int verse);
  void unload();
  void playPage(int number);
  void playChapter(int chapter);
  void playPart(int part);
  void stop();

signals:
  void installedChanged();
  void currentChanged();
  void playingChanged();
  void error(const QString& msg);
  void positionChanged(int chapter, int verse);
  void verseChanged();
  void chapterChanged();

  void settingsChanged();
  void downloaderChanged();
  void dataChanged();

private slots:
  void playerStateChanged();
  void policyAcquired();
  void policyLost();
  void policyDenied();
  void playerError();
  void playerMediaChanged();

private:
  void setChapter(int chapter);
  void setVerse(int verse);

  Settings *m_settings;
  DataProvider *m_data;

  QMap<int, Recitation *> m_installed;

  QMediaPlayer *m_player;
  MediaPlaylist *m_playlist;
  Recitation *m_recitation;
  int m_current;

  AudioPolicy *m_policy;
  bool m_play;

  int m_chapter;
  int m_verse;
};

#endif /* RECITATIONS_H */
