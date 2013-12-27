/*
 * Copyright (c) 2011-2013 Mohammed Sameer <msameer@foolab.org>.
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
#include "trans-meta.h"
#include <QLocale>
#include <QDebug>
#include "translation.h"
#include "downloader.h"
#include <QNetworkReply>
#include <QTemporaryFile>
#include "translation_p.h"
#include "settings.h"
#include "dataprovider.h"
#include "textprovider.h"
#include <QDebug>

#define INDEX_SUFFIX ".idx"
#define DATA_SUFFIX ".txt"
#define INDEX_FILTER "*" INDEX_SUFFIX

// TODO: limit the number of simultaneously downloaded translations.

Translations::Translations(QObject *parent)
  : QObject(parent), m_downloader(0), m_settings(0),
    m_data(0), m_current(0) {

}

Translations::~Translations() {
  qDeleteAll(m_info);
}

Settings *Translations::settings() const {
  return m_settings;
}

void Translations::setSettings(Settings *settings) {
  if (m_settings != settings) {
    m_settings = settings;
    emit settingsChanged();
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

TranslationPrivate *Translations::info(int tid) {
  foreach (TranslationPrivate *p, m_info) {
    if (p->tid() == tid) {
      return p;
    }
  }

  return 0;
}

int Translations::current() const {
  return m_current;
}

void Translations::setCurrent(int tid) {
  if (m_current != tid) {
    m_current = tid;

    emit currentChanged();
  }
}

void Translations::unload() {
  // TODO: what to do with current ?
  m_data->setSecondaryText(0);
}

bool Translations::load(int tid) {
  if (m_installed.isEmpty()) {
    return false;
  }

  if (m_installed.indexOf(tid) == -1) {
    return false;
  }

  TextProvider *s = m_data->secondaryTextProvider();
  if (s && s->id() == tid) {

    setCurrent(tid);

    return true;
  }

  TextProvider *p = new TextProvider(tid, data(tid), index(tid));
  if (!p->load()) {
    delete p;
    return false;
  }

  m_data->setSecondaryText(p);

  setCurrent(tid);

  m_settings->setDefaultTranslation(id(tid));

  return true;
}

bool Translations::loadDefault() {
  if (m_installed.isEmpty()) {
    return false;
  }

  QString id = m_settings->defaultTranslation();

  int t = id.isEmpty() ? m_installed.at(0) : tid(id);

  return load(t);
}

void Translations::refresh() {
  QDir dir(m_settings->translationsDir());
  dir.mkpath(".");

  QStringList list = dir.entryList(QStringList() << INDEX_FILTER,
				     QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

  m_installed.clear();

  foreach (const QString& file, list) {
    int tid = Translations::tid(QFileInfo(file).completeBaseName());
    if (m_installed.indexOf(tid) == -1) {
      m_installed << tid;
    }
  }

  emit installedChanged();

  emit activeChanged();
}

TranslationPrivate *Translations::registerTranslation(Translation *t) {
  TranslationPrivate *p = info(t->tid());

  if (!p) {
    p = new TranslationPrivate(m_installed.indexOf(t->tid()) == -1 ?
			       Translation::None : Translation::Installed, t->tid(), this);
    m_info << p;
  }

  p->add(t);

  return p;
}

void Translations::unregisterTranslation(Translation *t) {
  TranslationPrivate *p = t->d_ptr;

  if (!p) {
    return;
  }

  p->remove(t);

  if (p->canDestroy()) {
    m_info.takeAt(m_info.indexOf(p));
    delete p;
  }
}

QList<int> Translations::categories() const {
  QList<int> res;

  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    int language = Ts[x].language;
    if (res.indexOf(language) == -1) {
      res << language;
    }
  }

  qSort(res);

  return res;
}

QString Translations::categoryName(int category) {
  return QLocale::languageToString(static_cast<QLocale::Language>(category));
}

QList<int> Translations::translations(int category) {
  QList<int> res;

  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    if (Ts[x].language == category) {
      res << x;
    }
  }

  return res;
}

QString Translations::translationName(int translation) {
  return QString::fromUtf8(Ts[translation].name);
}

QString Translations::categoryNameForTranslation(int translation) {
  return categoryName(Ts[translation].language);
}

QList<int> Translations::installed() const {
  return m_installed;
}

QList<int> Translations::active() const {
  QList<int> res = m_installed;

  res += downloads();

  res += error();

  return res;
}

void Translations::startDownload(int tid) {
  TranslationPrivate *p = info(tid);
  if (!p || !p->canDownload()) {
    return;
  }

  QString url = QString("http://tanzil.net/trans/?transID=%1&type=txt").arg(id(tid));
  QNetworkReply *reply = m_downloader->get(url);

  if (!p->startDownload(reply)) {
    delete reply;
  }
}

void Translations::stopDownload(int tid) {
  TranslationPrivate *p = info(tid);
  if (!p) {
    return;
  }

  p->stopDownload();
}

void Translations::stopDownloads() {
  QList<int> d = downloads();

  foreach (int tid, d) {
    stopDownload(tid);
  }
}

QList<int> Translations::downloads() const {
  QList<int> res;

  foreach (const TranslationPrivate *p, m_info) {
    if (p->status() == Translation::Downloading) {
      res << p->tid();
    }
  }

  qSort(res);

  return res;
}

QList<int> Translations::error() const {
  QList<int> res;

  foreach (const TranslationPrivate *p, m_info) {
    if (p->status() == Translation::Error) {
      res << p->tid();
    }
  }

  qSort(res);

  return res;
}

void Translations::removeTranslation(int tid) {
  TranslationPrivate *p = info(tid);
  if (!p) {
    return;
  }

  p->stopDownload();

  p->remove();
}

QString Translations::id(int tid) const {
  return QString::fromUtf8(Ts[tid].id);
}

int Translations::tid(const QString& id) {
  for (int x = 0; x < TRANSLATIONS_LEN; x++) {
    if (QLatin1String(Ts[x].id) == id) {
      return x;
    }
  }

  return -1;
}

QString Translations::index(int tid) const {
  return QString("%1%2%3%4").arg(m_settings->translationsDir()).arg(QDir::separator())
    .arg(id(tid)).arg(INDEX_SUFFIX);
}

QString Translations::data(int tid) const {
  return QString("%1%2%3%4").arg(m_settings->translationsDir()).arg(QDir::separator())
    .arg(id(tid)).arg(DATA_SUFFIX);
}

void Translations::statusChanged(int tid, Translation::Status oldStatus,
				 Translation::Status newStatus) {

  TranslationPrivate *p = info(tid);
  if (!p) {
    return;
  }

  if (oldStatus == Translation::None && newStatus == Translation::Downloading) {
    // User initiated a download
    emit downloadsChanged();
    emit activeChanged();
  }
  else if (oldStatus == Translation::Installed && newStatus == Translation::None) {
    // User removed a translation
    m_installed.takeAt(m_installed.indexOf(p->tid()));
    emit installedChanged();
    emit activeChanged();
    emit removed(tid);
  }
  else if (oldStatus == Translation::Error && newStatus == Translation::Downloading) {
    // User restarted a failed download.
    emit activeChanged();
  }
  else if (oldStatus == Translation::Downloading && newStatus == Translation::None) {
    // User stopped a download
    emit downloadsChanged();
    emit activeChanged();
    emit removed(tid);
  }
  else if (oldStatus == Translation::Downloading && newStatus == Translation::Installed) {
    // Translation installed.
    m_installed.append(p->tid());
    emit installedChanged();
    emit activeChanged();
    emit installed(tid);
  }
  else if (oldStatus == Translation::Downloading && newStatus == Translation::Error) {
    // Error!
    emit failed(tid);
  }
}
