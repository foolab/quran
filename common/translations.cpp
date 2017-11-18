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
  QObject(parent),
  m_downloader(0),
  m_data(0) {

}

Translations::~Translations() {
  clear();
}

void Translations::clear() {
  std::for_each(m_translations.begin(), m_translations.end(),
		[](Translation *t) {t->stopDownload();});

  loadTranslation(QString());

  qDeleteAll(m_translations);
  m_translations.clear();

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

    m_translations.push_back(t);

    s.endGroup();

    // We do the signal connection at the end.
    // If we do it before checking installed ones then statusChanged() will be emitted
    // if we set any to installed. This will emit installedCountChanged()
    // We just emit the signal at the end as it's slightly more efficient
    QObject::connect(t, &Translation::statusChanged,
		     [t, this]() {
		       if (t->status() == Translation::Installed) {
			 emit installedCountChanged();
			 emit translationInstalled(t);
		       } else if (t->status() == Translation::Error) {
			 emit downloadError(t->name());
		       }
		     });
  }

  std::sort(m_translations.begin(), m_translations.end(),
	    [] (const Translation *a, const Translation *b) { return a->language() < b->language(); });
  emit installedCountChanged();
  emit refreshed();
}

int Translations::installedCount() const {
  return std::count_if(m_translations.begin(), m_translations.end(),
		       [](const Translation *t) {return t->status() == Translation::Installed;});
}

Translation *Translations::lookup(const QString& id) {
  auto iter = std::find_if(m_translations.begin(), m_translations.end(),
			   [&id](const Translation *t) {return t->uuid() == id;});

  return iter == m_translations.end() ? nullptr : (*iter);
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
  Translation *t = lookup(id);
  if (!t) {
    qmlInfo(this) << "Unknown translation " << id;
    return false;
  }

  t->stopDownload();

  QString index = indexPath(t->uuid());
  QString data = dataPath(t->uuid());

  if (QFile(index).remove() && QFile(data).remove()) {
    t->setStatus(Translation::None);
    emit installedCountChanged();
    emit translationRemoved(t);
    return true;
  }

  return false;
}

QString Translations::findInstalledTranslation() {
  auto iter =
    std::find_if(m_translations.begin(), m_translations.end(),
		 [](const Translation *t) {return t->status() == Translation::Installed;});

  return iter == m_translations.end() ? QString() : (*iter)->uuid();
}

bool Translations::loadTranslation(const QString& id) {
  if (id.isEmpty()) {
    TextProvider *p = m_data->secondaryTextProvider();
    if (p) {
      lookup(p->uuid())->setLoaded(false);
    }

    m_data->setSecondaryText(0);
    return true;
  }

  Translation *t = lookup(id);
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
    Translation *ot = lookup(old->uuid());
    if (ot) {
      ot->setLoaded(false);
    }
  }

  t->setLoaded(true);

  m_data->setSecondaryText(p);

  return true;
}

#if 0
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
#endif

bool Translations::isInstalled(Translation *t) {
  QFileInfo index(indexPath(t->uuid()));
  QFileInfo data(dataPath(t->uuid()));

  return index.size() > 0 && index.isReadable() && data.size() > 0 && data.isReadable();
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

Translation *Translations::translation(int index) {
  return m_translations[index];
}

int Translations::count() {
  return m_translations.size();
}
