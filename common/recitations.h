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

#include <QAbstractListModel>

class Settings;
class DataProvider;
class Recitation;
class Downloader;
class MediaPlayer;

class Recitations : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int installedCount READ installedCount NOTIFY installedCountChanged);
  Q_PROPERTY(Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged);
  Q_PROPERTY(Downloader *downloader READ downloader WRITE setDownloader NOTIFY downloaderChanged);
  Q_PROPERTY(DataProvider *data READ data WRITE setData NOTIFY dataChanged);
  Q_PROPERTY(MediaPlayer *player READ player WRITE setPlayer NOTIFY playerChanged);

  enum {
    RecitationRole = Qt::UserRole,
  };

public:
  Recitations(QObject *parent = 0);
  ~Recitations();

  Settings *settings() const;
  void setSettings(Settings *settings);

  DataProvider *data() const;
  void setData(DataProvider *data);

  Downloader *downloader() const;
  void setDownloader(Downloader *downloader);

  int installedCount() const;

  MediaPlayer *player() const;
  void setPlayer(MediaPlayer *player);

  Q_INVOKABLE bool loadRecitation(const QString& id);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
  void refresh();

signals:
  void installedCountChanged();
  void settingsChanged();
  void downloaderChanged();
  void dataChanged();
  void playerChanged();
  void refreshed();

private:
  void clear();
  int lookup(const QString& id);

  Settings *m_settings;
  DataProvider *m_data;
  Downloader *m_downloader;
  MediaPlayer *m_player;

  QList<Recitation *> m_recitations;

#ifdef QT_VERSION_5
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* RECITATIONS_H */
