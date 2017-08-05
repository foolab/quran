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
  foreach (Translation *t, m_translations) {
    t->stopDownload();
  }

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
    info->m_tid = translations.size();
    info->m_name = s.value("localizedName").toString();
    info->m_status = Translation::None;
    info->m_language = locale.language() == QLocale::C ? QLocale::English : locale.language();
    info->m_uuid = s.value("id").toString();

    Translation *t = new Translation(info, this);
    t->setDownloader(m_downloader);

    translations << t;

    s.endGroup();
  }

  // Now check which ones are installed
  QDir dir(m_dir);
  dir.mkpath(".");

  QStringList list =
    dir.entryList(QStringList() << INDEX_FILTER, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

  foreach (const QString& file, list) {
    int tid = lookup(QFileInfo(file).completeBaseName(), translations);
    if (tid == -1) {
      qmlInfo(this) << "Unknown translation " << file;
      continue;
    }

    translations[tid]->setStatus(Translation::Installed);
  }

  // We do the signal connection at the end.
  // If we do it before checking installed ones then statusChanged() will be emitted
  // if we set any to installed. This will trigger a call to translationStatusChanged()
  // Which will report an ugly "Unknown translation" because m_translations is empty.
  for (Translation *t : translations) {
    QObject::connect(t, SIGNAL(statusChanged()), this, SLOT(translationStatusChanged()));
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

QString Translations::translationId(int tid) const {
  return m_translations[tid]->uuid();
}

int Translations::lookup(const QString& id, const QList<Translation *>& translations) {
  auto iter = std::find_if(translations.constBegin(),
			   translations.constEnd(),
			   [&id](const Translation *t) {return t->uuid() == id;});

  return iter == translations.constEnd() ? -1 : (*iter)->tid();
}

QString Translations::indexPath(int tid) const {
  return QString("%1%2%3%4").arg(m_dir).arg(QDir::separator())
    .arg(translationId(tid)).arg(INDEX_SUFFIX);
}

QString Translations::dataPath(int tid) const {
  return QString("%1%2%3%4").arg(m_dir).arg(QDir::separator())
    .arg(translationId(tid)).arg(DATA_SUFFIX);
}

bool Translations::removeTranslation(const QString& id) {
  int tid = lookup(id, m_translations);
  if (tid == -1) {
    qmlInfo(this) << "Unknown translation " << id;
    return false;
  }

  Translation *t = m_translations[tid];
  t->stopDownload();

  QString index = indexPath(tid);
  QString data = dataPath(tid);

  if (QFile(index).remove() && QFile(data).remove()) {
    t->setStatus(Translation::None);
    int idx = m_translations.indexOf(t);

    if (idx != -1) {
      reportChanges(idx, idx);
    } else {
      // I doubt this could ever happen
      qmlInfo(this) << "Unknown translation installed";
      emit installedCountChanged();
    }

    return true;
  }

  return false;
}

bool Translations::loadTranslation(const QString& id) {
  if (id.isEmpty()) {
    TextProvider *p = m_data->secondaryTextProvider();
    if (p) {
      m_translations[p->id()]->setLoaded(false);
    }

    m_data->setSecondaryText(0);
    return true;
  }

  int tid = lookup(id, m_translations);
  if (tid == -1) {
    qmlInfo(this) << "Unknown translation " << id;
    return false;
  }

  Translation *t = m_translations[tid];
  if (t->status() != Translation::Installed) {
    qmlInfo(this) << "Translation " << id << " is not installed";
    return false;
  }

  TextProvider *p = m_data->secondaryTextProvider();
  if (p && p->id() == tid) {
    return true;
  }

  p = new TextProvider(tid, dataPath(tid), indexPath(tid));
  if (!p->load()) {
    qmlInfo(this) << "Failed to load translation " << tid;
    delete p;
    return false;
  }

  TextProvider *old = m_data->secondaryTextProvider();
  if (old) {
    m_translations[old->id()]->setLoaded(false);
  }

  m_translations[tid]->setLoaded(true);

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

void Translations::translationStatusChanged() {
  if (Translation *t = dynamic_cast<Translation *>(sender())) {
    if (t->status() == Translation::Installed) {
      int idx = m_translations.indexOf(t);
      if (idx != -1) {
	reportChanges(idx, idx);
      } else {
	// I doubt this could ever happen
	qmlInfo(this) << "Unknown translation";
	emit installedCountChanged();
      }
    } else if (t->status() == Translation::Error) {
      emit downloadError(t->name());
    }
  }
}

void Translations::reportChanges(int from, int to) {
  emit QAbstractItemModel::dataChanged(index(from, 0), index(to, 0));
  emit installedCountChanged();
}

QHash<int, QByteArray> Translations::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TranslationRole] = "translation";
  roles[LanguageRole] = "language";

  return roles;
}
