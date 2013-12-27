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

#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <QObject>
#include <QStringList>
#include "translation.h"
#include <QDir>

class Downloader;
class TranslationPrivate;
class Settings;
class DataProvider;

class Translations : public QObject {
  Q_OBJECT

  Q_PROPERTY(QList<int> installed READ installed NOTIFY installedChanged);
  Q_PROPERTY(QList<int> downloads READ downloads NOTIFY downloadsChanged);
  Q_PROPERTY(QList<int> categories READ categories NOTIFY categoriesChanged);
  Q_PROPERTY(QList<int> active READ active NOTIFY activeChanged);

  Q_PROPERTY(int current READ current NOTIFY currentChanged);
  Q_PROPERTY(Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged);
  Q_PROPERTY(Downloader *downloader READ downloader WRITE setDownloader NOTIFY downloaderChanged);
  Q_PROPERTY(DataProvider *data READ data WRITE setData NOTIFY dataChanged);

public:
  Translations(QObject *parent = 0);
  ~Translations();

  QList<int> installed() const;
  QList<int> downloads() const;
  QList<int> categories() const;
  QList<int> active() const;
  QList<int> error() const;

  int current() const;

  Q_INVOKABLE QString categoryName(int category);
  Q_INVOKABLE QList<int> translations(int category);
  Q_INVOKABLE QString translationName(int translation);

  Q_INVOKABLE QString categoryNameForTranslation(int translation);

  void statusChanged(int tid, Translation::Status oldStatus, Translation::Status newStatus);

  Q_INVOKABLE bool load(int tid);
  Q_INVOKABLE bool loadDefault();

  TranslationPrivate *registerTranslation(Translation *t);
  void unregisterTranslation(Translation *t);

  QString index(int tid) const;
  QString data(int tid) const;

  Q_INVOKABLE QString id(int tid) const;

  Settings *settings() const;
  void setSettings(Settings *settings);

  Downloader *downloader() const;
  void setDownloader(Downloader *downloader);

  DataProvider *data() const;
  void setData(DataProvider *data);

public slots:
  void refresh();
  void startDownload(int tid);
  void stopDownload(int tid);
  void removeTranslation(int tid);
  void unload();
  void stopDownloads();

signals:
  void downloadsChanged();
  void categoriesChanged();
  void activeChanged();
  void installedChanged();
  void currentChanged();

  void installed(int id);
  void failed(int id);
  void removed(int id);

  void settingsChanged();
  void downloaderChanged();
  void dataChanged();

private:
  TranslationPrivate *info(int tid);
  int tid(const QString& id);

  void setCurrent(int tid);

  Downloader *m_downloader;

  Settings *m_settings;
  DataProvider *m_data;

  QList<TranslationPrivate *> m_info;

  QList<int> m_installed;

  int m_current;
};

#endif /* TRANSLATIONS_H */
