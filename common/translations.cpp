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

#include "translations.h"
#include "translation.h"
#include "dataprovider.h"
#include "textprovider.h"
#include <QQmlInfo>
#include <QQmlEngine>
#include <QDir>
#include <algorithm>
#include <QSettings>

#define INDEX_SUFFIX ".idx"
#define DATA_SUFFIX ".txt"
#define INDEX_FILTER "*" INDEX_SUFFIX

// TODO: limit the number of simultaneously downloaded translations.

Translations::Translations(QObject *parent) :
  QAbstractListModel(parent),
  m_downloader(0),
  m_data(0) {

}

Translations::~Translations() {
  clear();
}

void Translations::clear() {
  std::for_each(m_translations.constBegin(), m_translations.constEnd(),
		[](Translation *t) {t->stopDownload();});

  loadTranslation(QString());

  bool emitSignal = !m_translations.isEmpty();

  if (emitSignal) {
    beginRemoveRows(QModelIndex(), 0, m_translations.size() - 1);
  }

  qDeleteAll(m_translations);
  m_translations.clear();

  if (emitSignal) {
    endRemoveRows();
  }

  emit installedCountChanged();
}

QString Translations::dir() const {
  return m_dir;
}

void Translations::setDir(const QString& dir) {
  if (m_dir != dir) {
    m_dir = dir;
    emit dirChanged();
  }
}

Downloader *Translations::downloader() const {
  return m_downloader;
}

void Translations::setDownloader(Downloader *downloader) {
  if (m_downloader != downloader) {
    m_downloader = downloader;
    emit downloaderChanged();
  }
}

DataProvider *Translations::data() const {
  return m_data;
}

void Translations::setData(DataProvider *data) {
  if (m_data != data) {
    m_data = data;
    emit dataChanged();
  }
}

void Translations::refresh() {
  clear();

  QList<Translation *> translations;

  QSettings s(":/data/translations.ini", QSettings::IniFormat);
  s.setIniCodec("UTF-8");

  for (const QString& id : s.childGroups()) {
    s.beginGroup(id);
    QLocale locale(s.value("language").toString());

    TranslationInfo *info = new TranslationInfo;
    info->m_name = s.value("localizedName").toString();
    info->m_status = Translation::None;
    info->m_language = locale.language() == QLocale::C ? QLocale::English : locale.language();
    info->m_uuid = s.value("id").toString();

    Translation *t = new Translation(info, this);
    t->setDownloader(m_downloader);

    if (isInstalled(t)) {
      t->setStatus(Translation::Installed);
    }

    translations << t;

    s.endGroup();

    // We do the signal connection at the end.
    // If we do it before checking installed ones then statusChanged() will be emitted
    // if we set any to installed. This will trigger a call to reportChanges()
    // Which will report an ugly "Unknown translation" because m_translations is empty.
    QObject::connect(t, &Translation::statusChanged,
		     [t, this]() {
		       if (t->status() == Translation::Installed) {
			 reportChanges(t);
		       } else if (t->status() == Translation::Error) {
			 emit downloadError(t->name());
		       }
		     });
  }

  beginInsertRows(QModelIndex(), 0, translations.size() - 1);
  m_translations = translations;
  endInsertRows();

  emit installedCountChanged();
  emit refreshed();
}

int Translations::installedCount() const {
  return std::count_if(m_translations.constBegin(),
		m_translations.constEnd(),
		[](const Translation *t) {return t->status() == Translation::Installed;});
}

Translation *Translations::lookup(const QString& id, const QList<Translation *>& translations) {
  auto iter = std::find_if(translations.constBegin(),
			   translations.constEnd(),
			   [&id](const Translation *t) {return t->uuid() == id;});

  return iter == translations.constEnd() ? nullptr : (*iter);
}

QString Translations::indexPath(const QString& id) const {
  ensureDir();

  return QString("%1%2%3%4").arg(m_dir).arg(QDir::separator())
    .arg(id).arg(INDEX_SUFFIX);
}

QString Translations::dataPath(const QString& id) const {
  ensureDir();

  return QString("%1%2%3%4").arg(m_dir).arg(QDir::separator())
    .arg(id).arg(DATA_SUFFIX);
}

bool Translations::removeTranslation(const QString& id) {
  Translation *t = lookup(id, m_translations);
  if (!t) {
    qmlInfo(this) << "Unknown translation " << id;
    return false;
  }

  t->stopDownload();

  QString index = indexPath(t->uuid());
  QString data = dataPath(t->uuid());

  if (QFile(index).remove() && QFile(data).remove()) {
    t->setStatus(Translation::None);
    reportChanges(t);
    return true;
  }

  return false;
}

bool Translations::loadTranslation(const QString& id) {
  if (id.isEmpty()) {
    TextProvider *p = m_data->secondaryTextProvider();
    if (p) {
      lookup(p->uuid(), m_translations)->setLoaded(false);
    }

    m_data->setSecondaryText(0);
    return true;
  }

  Translation *t = lookup(id, m_translations);
  if (!t) {
    qmlInfo(this) << "Unknown translation " << id;
    return false;
  }

  if (t->status() != Translation::Installed) {
    qmlInfo(this) << "Translation " << id << " is not installed";
    return false;
  }

  TextProvider *p = m_data->secondaryTextProvider();
  if (p && p->uuid() == t->uuid()) {
    return true;
  }

  p = new TextProvider(-1, t->uuid(), dataPath(t->uuid()), indexPath(t->uuid()));
  if (!p->load()) {
    qmlInfo(this) << "Failed to load translation " << t->uuid();
    delete p;
    return false;
  }

  TextProvider *old = m_data->secondaryTextProvider();
  if (old) {
    Translation *ot = lookup(old->uuid(), m_translations);
    if (ot) {
      ot->setLoaded(false);
    }
  }

  t->setLoaded(true);

  m_data->setSecondaryText(p);

  return true;
}

int Translations::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_translations.size();
  }

  return 0;
}

QVariant Translations::data(const QModelIndex& index, int role) const {
  if (index.row() >= 0 && index.row() < m_translations.size()) {
    QObject *translation = m_translations[index.row()];

    switch (role) {
    case LanguageRole:
      return dynamic_cast<Translation *>(translation)->language();

    case TranslationRole:
      QQmlEngine::setObjectOwnership(translation, QQmlEngine::CppOwnership);
      return QVariant::fromValue<QObject *>(translation);

    default:
      break;
    }
  }

  return QVariant();
}

void Translations::reportChanges(int idx) {
  emit QAbstractItemModel::dataChanged(index(idx, 0), index(idx, 0));
  emit installedCountChanged();
}

QHash<int, QByteArray> Translations::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TranslationRole] = "translation";
  roles[LanguageRole] = "language";

  return roles;
}

bool Translations::isInstalled(Translation *t) {
  QFileInfo index(indexPath(t->uuid()));
  QFileInfo data(dataPath(t->uuid()));

  return index.size() > 0 && index.isReadable() && data.size() > 0 && data.isReadable();
}

void Translations::reportChanges(Translation *t) {
  int idx = m_translations.indexOf(t);

  if (idx != -1) {
    reportChanges(idx);
  } else {
    // I doubt this could ever happen
    qmlInfo(this) << "Unknown translation installed";
    emit installedCountChanged();
  }
}

void Translations::ensureDir() const {
  // Create the directory just in case:
  QDir dir(m_dir);
  if (!dir.exists()) {
    if (!dir.mkpath(".")) {
      qWarning() << "Failed to create dir" << m_dir;
    }
  }
}
