/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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
#include <deque>

class Recitation;
class MediaPlayback;

class Recitations : public QObject {
  Q_OBJECT

  Q_PROPERTY(int installedCount READ installedCount NOTIFY installedCountChanged);
  Q_PROPERTY(QString dir READ dir WRITE setDir NOTIFY dirChanged);
  Q_PROPERTY(MediaPlayback *player READ player WRITE setPlayer NOTIFY playerChanged);

public:
  Recitations(QObject *parent = 0);
  ~Recitations();

  QString dir() const;
  void setDir(const QString& dir);

  int installedCount() const;

  MediaPlayback *player() const;
  void setPlayer(MediaPlayback *player);

  Q_INVOKABLE bool loadRecitation(const QString& id);

  int count();
  Recitation *recitation(int index);

public slots:
  void refresh();

signals:
  void installedCountChanged();
  void dirChanged();
  void playerChanged();
  void refreshed();
  void recitationEnabled(Recitation *r);
  void recitationDisabled(Recitation *r);

private:
  void clear();
  Recitation *lookup(const QString& id);

  QString m_dir;
  MediaPlayback *m_player;

  std::deque<Recitation *> m_recitations;
};

#endif /* RECITATIONS_H */
