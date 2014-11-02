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

#include "recitations.h"
#include "recitation.h"
#include "settings.h"
#include "recite-meta.h"
#ifdef QT_VERSION_5
#include <QQmlInfo>
#include <QQmlEngine>
#else
#include <QDeclarativeInfo>
#include <QDeclarativeEngine>
#endif
#include <QDir>
#include "mediaplayer.h"

Recitations::Recitations(QObject *parent)
  : QAbstractListModel(parent),
    m_settings(0),
    m_player(0) {

  QHash<int, QByteArray> roles;

  roles[RecitationRole] = "recitation";
  roles[OnlineRole] = "online";

  setRoleNames(roles);
}

Recitations::~Recitations() {
  clear();
}

void Recitations::clear() {
  // unload recitation
  loadRecitation(QString());

  bool emitSignal = !m_recitations.isEmpty();
  if (emitSignal) {
    beginRemoveRows(QModelIndex(), 0, m_recitations.size() - 1);
  }

  qDeleteAll(m_recitations);
  m_recitations.clear();

  if (emitSignal) {
    endRemoveRows();
  }

  emit installedCountChanged();
}

Settings *Recitations::settings() const {
  return m_settings;
}

void Recitations::setSettings(Settings *settings) {
  if (m_settings != settings) {
    m_settings = settings;
    emit settingsChanged();
  }
}

MediaPlayer *Recitations::player() const {
  return m_player;
}

void Recitations::setPlayer(MediaPlayer *player) {
  if (m_player != player) {
    m_player = player;
    emit playerChanged();
  }
}

void Recitations::refresh() {
  clear();

  QList<Recitation *> recitations;

  // Create entries for all our known recitations
  for (int x = 0; x < RECITATIONS_LEN; x++) {
    RecitationInfo *info = new RecitationInfo;
    info->m_type = Recitation::Online,
    info->m_id = x;
    info->m_uuid = QString::fromUtf8(Rs[x].id);
    info->m_name = QString::fromUtf8(Rs[x].translated_name);
    info->m_quality = QString::fromUtf8(Rs[x].quality);
    info->m_dir =
      QString("%1%2%3%2").arg(m_settings->recitationsDir()).arg(QDir::separator()).arg(info->m_uuid);
    info->m_url = QString::fromUtf8(Rs[x].url);
    info->m_status = Recitation::None;

    recitations << new Recitation(info, this);
  }

  QDir dir(m_settings->recitationsDir());
  dir.mkpath(".");

  QStringList entries(dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

  foreach (const QString& entry, entries) {
    QString recitationDir =
      QString("%1%2%3%2").arg(m_settings->recitationsDir()).arg(QDir::separator()).arg(entry);
    QString id = entry;

    RecitationInfo *info = Recitation::guessType(recitationDir);
    if (!info) {
      qmlInfo(this) << "Invalid recitation in " << recitationDir;
      continue;
    }

    info->m_uuid = id;

    // If it's an online recitation and we know about it then update the existing
    if (info->m_type == Recitation::Online) {
      int x = lookup(info->m_uuid);
      if (x == -1) {
	// That is strange but we will just load it.
	info->m_id = recitations.size();
	recitations << new Recitation(info, this);
	recitations[info->m_id]->setStatus(Recitation::Installed);
      } else {
	recitations[x]->setStatus(Recitation::Installed);
	delete info;
      }
    } else {
      info->m_id = recitations.size();
      recitations << new Recitation(info, this);
      recitations[info->m_id]->setStatus(Recitation::Installed);
    }
  }

  // zipped zekr
  entries = dir.entryList(QStringList() << "*.zip", QDir::Files | QDir::NoDotAndDotDot);
  foreach (const QString& entry, entries) {
    QString recitationDir =
      QString("%1%2%3").arg(m_settings->recitationsDir()).arg(QDir::separator()).arg(entry);
    QString id = entry;

    RecitationInfo *info = Recitation::guessType(recitationDir);
    if (!info) {
      qmlInfo(this) << "Invalid recitation in " << recitationDir;
      continue;
    }

    info->m_uuid = id;
    info->m_id = recitations.size();
    recitations << new Recitation(info, this);
    recitations[info->m_id]->setStatus(Recitation::Installed);
  }

  foreach (Recitation *r, recitations) {
    QObject::connect(r, SIGNAL(enabled()), this, SIGNAL(installedCountChanged()));
    QObject::connect(r, SIGNAL(disabled()), this, SIGNAL(installedCountChanged()));
  }

  beginInsertRows(QModelIndex(), 0, recitations.size() - 1);
  m_recitations = recitations;
  endInsertRows();

  emit installedCountChanged();
  emit refreshed();
}

bool Recitations::loadRecitation(const QString& id) {
  if (id.isEmpty()) {
    if (Recitation *r = m_player->recitation()) {
      r->setLoaded(false);
    }

    m_player->setRecitation(0);
    return true;
  }

  int x = -1;
  // lookup() checks built ins only
  foreach (Recitation *r, m_recitations) {
    if (r->uuid() == id) {
      x = r->rid();
      break;
    }
  }

  if (x == -1) {
    qmlInfo(this) << "Unknown recitation " << id;
    return false;
  }

  if (m_recitations[x]->status() == Recitation::Installed) {
    m_player->setRecitation(m_recitations[x]);
    m_recitations[x]->setLoaded(true);
    return true;
  }

  qmlInfo(this) << "Recitation " << id << " is not enabled";

  return false;
}

int Recitations::lookup(const QString& id) {
  for (int x = 0; x < RECITATIONS_LEN; x++) {
    if (QLatin1String(Rs[x].id) == id) {
      return x;
    }
  }

  return -1;
}

int Recitations::installedCount() const {
  int count = 0;
  foreach (Recitation *r, m_recitations) {
    if (r->status() == Recitation::Installed) {
      ++count;
    }
  }

  return count;
}

int Recitations::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_recitations.size();
  }

  return 0;
}

QVariant Recitations::data(const QModelIndex& index, int role) const {
  if (index.row() >= 0 && index.row() < m_recitations.size()) {
    QObject *recitation = m_recitations[index.row()];

    switch (role) {
    case OnlineRole:
      return dynamic_cast<Recitation *>(recitation)->type() == Recitation::Online ? "online" : "offline";

    case RecitationRole:
#ifdef QT_VERSION_5
      QQmlEngine::setObjectOwnership(recitation, QQmlEngine::CppOwnership);
#else
      QDeclarativeEngine::setObjectOwnership(recitation, QDeclarativeEngine::CppOwnership);
#endif
      return QVariant::fromValue<QObject *>(recitation);

    default:
      break;
    }
  }

  return QVariant();
}

#ifdef QT_VERSION_5
QHash<int, QByteArray> Recitations::roleNames() const {
  return m_roles;
}

void Recitations::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
