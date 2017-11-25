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
  : QObject(parent),
    m_player(0) {

}

Recitations::~Recitations() {
  clear();
}

void Recitations::clear() {
  // unload recitation
  loadRecitation(QString());

  qDeleteAll(m_recitations);
  m_recitations.clear();

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

    m_recitations.push_back(new Recitation(info, this));

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
      Recitation *r = lookup(info->m_uuid);
      if (!r) {
	// That is strange but we will just load it.
	Recitation *r2 = new Recitation(info, this);
	r2->setStatus(Recitation::Installed);
	m_recitations.push_back(r2);
      } else {
	r->setStatus(info->m_status);
	delete info;
      }
    } else {
      Recitation *r = new Recitation(info, this);
      r->setStatus(Recitation::Installed);
      m_recitations.push_back(r);
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
    Recitation *r = new Recitation(info, this);
    r->setStatus(Recitation::Installed);
    m_recitations.push_back(r);
  }

  for (Recitation *r : m_recitations) {
    QObject::connect(r, &Recitation::enabled,
		     [r, this] {
		       emit installedCountChanged();
		       emit recitationEnabled(r);
		     });

    QObject::connect(r, &Recitation::disabled,
		     [r, this] {
		       emit installedCountChanged();
		       emit recitationDisabled(r);
		     });
  }

  std::sort(m_recitations.begin(), m_recitations.end(),
	    [] (const Recitation *a, const Recitation *b) { return a->name() < b->name(); });

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

  Recitation *r = lookup(id);

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

Recitation *Recitations::lookup(const QString& id) {
  auto iter = std::find_if(m_recitations.begin(), m_recitations.end(),
			   [&id](const Recitation *r) {return r->uuid() == id;});

  return iter == m_recitations.end() ? nullptr : (*iter);
}

int Recitations::installedCount() const {
  return std::count_if(m_recitations.begin(),
		       m_recitations.end(),
		       [](const Recitation *r) {return r->status() == Recitation::Installed;});
}

int Recitations::count() {
  return m_recitations.size();
}

Recitation *Recitations::recitation(int index) {
  return m_recitations[index];
}
