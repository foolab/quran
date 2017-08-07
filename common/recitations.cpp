/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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
#include <QSettings>
#include <QQmlInfo>
#include <QQmlEngine>
#include <QDir>
#include "mediaplayer.h"
#include <algorithm>

Recitations::Recitations(QObject *parent)
  : QAbstractListModel(parent),
    m_player(0) {

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

QString Recitations::dir() const {
  return m_dir;
}

void Recitations::setDir(const QString& dir) {
  if (m_dir != dir) {
    m_dir = dir;
    emit dirChanged();
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

  QSettings s(":/data/recitations.ini", QSettings::IniFormat);
  s.setIniCodec("UTF-8");

  // Create entries for all our known recitations
  for (const QString& id : s.childGroups()) {
    s.beginGroup(id);

    RecitationInfo *info = new RecitationInfo;
    info->m_type = Recitation::Online;
    info->m_uuid = id;
    info->m_name = s.value("reciterArabic").toString();
    info->m_quality = s.value("quality").toString();
    info->m_dir =
      QString("%1%2%3%2").arg(m_dir).arg(QDir::separator()).arg(info->m_uuid);
    info->m_url = s.value("audioUrl").toString();
    info->m_status = Recitation::None;

    recitations << new Recitation(info, this);

    s.endGroup();
  }

  QDir dir(m_dir);
  dir.mkpath(".");

  QStringList entries(dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

  foreach (const QString& entry, entries) {
    QString recitationDir =
      QString("%1%2%3%2").arg(m_dir).arg(QDir::separator()).arg(entry);
    QString id = entry;

    RecitationInfo *info = Recitation::guessType(recitationDir);
    if (!info) {
      qmlInfo(this) << "Invalid recitation in " << recitationDir;
      continue;
    }

    info->m_uuid = id;

    // If it's an online recitation and we know about it then update the existing
    if (info->m_type == Recitation::Online) {
      Recitation *r = lookup(info->m_uuid, recitations);
      if (!r) {
	// That is strange but we will just load it.
	recitations << new Recitation(info, this);
	recitations.last()->setStatus(Recitation::Installed);
      } else {
	r->setStatus(Recitation::Installed);
	delete info;
      }
    } else {
      recitations << new Recitation(info, this);
      recitations.last()->setStatus(Recitation::Installed);
    }
  }

  // zipped zekr
  entries = dir.entryList(QStringList() << "*.zip", QDir::Files | QDir::NoDotAndDotDot);
  foreach (const QString& entry, entries) {
    QString recitationDir =
      QString("%1%2%3").arg(m_dir).arg(QDir::separator()).arg(entry);
    QString id = entry;

    RecitationInfo *info = Recitation::guessType(recitationDir);
    if (!info) {
      qmlInfo(this) << "Invalid recitation in " << recitationDir;
      continue;
    }

    info->m_uuid = id;
    recitations << new Recitation(info, this);
    recitations.last()->setStatus(Recitation::Installed);
  }

  for (Recitation *r : recitations) {
    QObject::connect(r, SIGNAL(enabled()), this, SLOT(reportChanges()));
    QObject::connect(r, SIGNAL(disabled()), this, SLOT(reportChanges()));
  }

  beginInsertRows(QModelIndex(), 0, recitations.size() - 1);
  m_recitations = recitations;
  endInsertRows();

  emit installedCountChanged();
  emit refreshed();
}

bool Recitations::loadRecitation(const QString& id) {
  Recitation *old = m_player->recitation();

  if (id.isEmpty()) {
    if (old) {
      old->setLoaded(false);
    }

    m_player->setRecitation(0);
    return true;
  }

  Recitation *r = lookup(id, m_recitations);

  if (!r) {
    qmlInfo(this) << "Unknown recitation " << id;
    return false;
  }

  if (r->status() == Recitation::Installed) {
    if (old) {
      old->setLoaded(false);
    }

    m_player->setRecitation(r);
    r->setLoaded(true);
    return true;
  }

  qmlInfo(this) << "Recitation " << id << " is not enabled";

  return false;
}

Recitation *Recitations::lookup(const QString& id, const QList<Recitation *>& recitations) {
  auto iter = std::find_if(recitations.constBegin(), recitations.constEnd(),
			   [&id](const Recitation *r) {return r->uuid() == id;});

  return iter == recitations.constEnd() ? nullptr : (*iter);
}

int Recitations::installedCount() const {
  return std::count_if(m_recitations.constBegin(),
		m_recitations.constEnd(),
		[](const Recitation *r) {return r->status() == Recitation::Installed;});
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
      QQmlEngine::setObjectOwnership(recitation, QQmlEngine::CppOwnership);
      return QVariant::fromValue<QObject *>(recitation);

    default:
      break;
    }
  }

  return QVariant();
}

void Recitations::reportChanges() {
  if (Recitation *r = dynamic_cast<Recitation *>(sender())) {
    int idx = m_recitations.indexOf(r);
    if (idx != -1) {
      emit QAbstractItemModel::dataChanged(index(idx, 0), index(idx, 0));
    } else {
      // I doubt this could ever happen
      qmlInfo(this) << "Unknown recitation";
    }

    emit installedCountChanged();
  }
}

QHash<int, QByteArray> Recitations::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[RecitationRole] = "recitation";
  roles[OnlineRole] = "online";

  return roles;
}
