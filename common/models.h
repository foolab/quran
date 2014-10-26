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

#ifndef MODELS_H
#define MODELS_H

#include <QAbstractListModel>
#include <QStringList>

class Recitations;

class RecitationModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Recitations *recitations READ recitations WRITE setRecitations NOTIFY recitationsChanged);

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    NameRole,
    OnlineRole,
  };

  RecitationModel(QObject *parent = 0);
  virtual ~RecitationModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Recitations *recitations() const;
  void setRecitations(Recitations *recitations);

signals:
  void recitationsChanged();

protected slots:
  void addId(const QString& id);
  void removeId(const QString& id);
  void refresh();

protected:
  void recitationsUpdated();

  void setIds(const QStringList& ids);

  Recitations *m_recitations;

private:
#ifdef QT_VERSION_5
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif

  QStringList m_ids;
};

class InstallableRecitationsModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Recitations *recitations READ recitations WRITE setRecitations NOTIFY recitationsChanged);

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    NameRole,
    QualityRole,
    InstalledRole,
  };

  InstallableRecitationsModel(QObject *parent = 0);
  virtual ~InstallableRecitationsModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Recitations *recitations() const;
  void setRecitations(Recitations *recitations);

signals:
  void recitationsChanged();

protected slots:
  void addId(const QString& id);
  void removeId(const QString& id);
  void refresh();

protected:
  void recitationsUpdated();

  void setIds(const QStringList& ids);

  Recitations *m_recitations;

private:
#ifdef QT_VERSION_5
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif

  QStringList m_ids;
};

#endif /* MODELS_H */
