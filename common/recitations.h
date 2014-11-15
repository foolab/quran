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

class Recitation;
class MediaPlayer;

class Recitations : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int installedCount READ installedCount NOTIFY installedCountChanged);
  Q_PROPERTY(QString dir READ dir WRITE setDir NOTIFY dirChanged);
  Q_PROPERTY(MediaPlayer *player READ player WRITE setPlayer NOTIFY playerChanged);

  enum {
    RecitationRole = Qt::UserRole,
    OnlineRole = Qt::UserRole + 1,
  };

public:
  Recitations(QObject *parent = 0);
  ~Recitations();

  QString dir() const;
  void setDir(const QString& dir);

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
  void dirChanged();
  void playerChanged();
  void refreshed();

private slots:
  void reportChanges();

private:
  void clear();
  int lookup(const QString& id);

  QString m_dir;
  MediaPlayer *m_player;

  QList<Recitation *> m_recitations;

#ifdef QT_VERSION_5
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* RECITATIONS_H */
