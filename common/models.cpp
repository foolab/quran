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

#include "models.h"
#include "recitations.h"
#include "recitation.h"
#include <QDebug>

RecitationModel::RecitationModel(QObject *parent) :
  QAbstractListModel(parent),
  m_recitations(0) {

  QHash<int, QByteArray> roles;
  roles[IdRole] = "recitationId";
  roles[NameRole] = "name";
  roles[OnlineRole] = "isOnline";

  setRoleNames(roles);
}

RecitationModel::~RecitationModel() {

}

int RecitationModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_ids.size();
  }

  return 0;
}

QVariant RecitationModel::data(const QModelIndex& index, int role) const {
  if (index.row() < m_ids.size()) {
    switch (role) {
    case IdRole:
      return m_ids[index.row()];
    case NameRole:
      return m_recitations->recitation(m_ids[index.row()])->name();
    case OnlineRole:
      return m_recitations->recitation(m_ids[index.row()])->isOnline();
    }
  }

  return QVariant();
}

Recitations *RecitationModel::recitations() const {
  return m_recitations;
}

void RecitationModel::setRecitations(Recitations *recitations) {
  if (m_recitations != recitations) {

    m_recitations = recitations;

    emit recitationsChanged();

    recitationsUpdated();
  }
}

void RecitationModel::addId(const QString& id) {
  if (m_ids.contains(id)) {
    qCritical() << "id already known" << id;
    return;
  }

  beginInsertRows(QModelIndex(), m_ids.size(), m_ids.size());
  m_ids << id;
  endInsertRows();
}

void RecitationModel::removeId(const QString& id) {
  int index = m_ids.indexOf(id);
  if (index == -1) {
    qCritical() << "unknown id" << id;
    return;
  }

  beginRemoveRows(QModelIndex(), index, index);
  m_ids.removeAt(index);
  endRemoveRows();
}

void RecitationModel::setIds(const QStringList& ids) {
  if (!m_ids.isEmpty()) {
    beginRemoveRows(QModelIndex(), 0, m_ids.size() - 1);
    m_ids.clear();
    endRemoveRows();
  }

  if (!ids.isEmpty()) {
    beginInsertRows(QModelIndex(), 0, ids.size() - 1);
    m_ids = ids;
    endInsertRows();
  }
}

void RecitationModel::recitationsUpdated() {
  QObject::connect(m_recitations, SIGNAL(added(const QString&)),
		   this, SLOT(addId(const QString&)));
  QObject::connect(m_recitations, SIGNAL(removed(const QString&)),
		   this, SLOT(removeId(const QString&)));
  QObject::connect(m_recitations, SIGNAL(refreshed()), this, SLOT(refresh()));

  refresh();
}

void RecitationModel::refresh() {
  setIds(m_recitations->installed());
}

#ifdef QT_VERSION_5
QHash<int, QByteArray> RecitationModel::roleNames() const {
  return m_roles;
}

void RecitationModel::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif

InstallableRecitationsModel::InstallableRecitationsModel(QObject *parent) :
  QAbstractListModel(parent),
  m_recitations(0) {

  QHash<int, QByteArray> roles;
  roles[IdRole] = "recitationId";
  roles[NameRole] = "name";
  roles[QualityRole] = "quality";
  roles[InstalledRole] = "installed";

  setRoleNames(roles);
}

InstallableRecitationsModel::~InstallableRecitationsModel() {

}

int InstallableRecitationsModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_ids.size();
  }

  return 0;
}

QVariant InstallableRecitationsModel::data(const QModelIndex& index, int role) const {
  if (index.row() >= rowCount()) {
    return QVariant();
  }

  if (index.row() < m_ids.size()) {
    switch (role) {
    case IdRole:
      return m_ids[index.row()];
    case NameRole:
      return m_recitations->installableName(m_ids[index.row()]);
    case QualityRole:
      return m_recitations->installableQuality(m_ids[index.row()]);
    case InstalledRole:
      return m_recitations->installableIsInstalled(m_ids[index.row()]);
    }
  }

  return QVariant();
}

Recitations *InstallableRecitationsModel::recitations() const {
  return m_recitations;
}

void InstallableRecitationsModel::setRecitations(Recitations *recitations) {
  if (m_recitations != recitations) {

    m_recitations = recitations;

    emit recitationsChanged();

    recitationsUpdated();
  }
}

void InstallableRecitationsModel::addId(const QString& id) {
  int index = m_ids.indexOf(id);
  if (index == -1) {
    qCritical() << "id already known" << id;
    return;
  }

  QModelIndex idx = QAbstractListModel::index(index, 0, QModelIndex());
  emit dataChanged(idx, idx);
}

void InstallableRecitationsModel::removeId(const QString& id) {
  int index = m_ids.indexOf(id);
  if (index == -1) {
    qCritical() << "unknown id" << id;
    return;
  }

  QModelIndex idx = QAbstractListModel::index(index, 0, QModelIndex());
  emit dataChanged(idx, idx);
}

void InstallableRecitationsModel::setIds(const QStringList& ids) {
  if (!m_ids.isEmpty()) {
    beginRemoveRows(QModelIndex(), 0, m_ids.size() - 1);
    m_ids.clear();
    endRemoveRows();
  }

  if (!ids.isEmpty()) {
    beginInsertRows(QModelIndex(), 0, ids.size() - 1);
    m_ids = ids;
    endInsertRows();
  }
}

void InstallableRecitationsModel::recitationsUpdated() {
  QObject::connect(m_recitations, SIGNAL(added(const QString&)),
		   this, SLOT(addId(const QString&)));
  QObject::connect(m_recitations, SIGNAL(removed(const QString&)),
		   this, SLOT(removeId(const QString&)));

  refresh();
}

void InstallableRecitationsModel::refresh() {
  setIds(m_recitations->installable());
}

#ifdef QT_VERSION_5
QHash<int, QByteArray> InstallableRecitationsModel::roleNames() const {
  return m_roles;
}

void InstallableRecitationsModel::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
