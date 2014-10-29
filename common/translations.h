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

#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <QAbstractListModel>

class Downloader;
class Settings;
class DataProvider;
class Translation;

class Translations : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int installedCount READ installedCount NOTIFY installedCountChanged);
  Q_PROPERTY(Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged);
  Q_PROPERTY(Downloader *downloader READ downloader WRITE setDownloader NOTIFY downloaderChanged);
  Q_PROPERTY(DataProvider *data READ data WRITE setData NOTIFY dataChanged);

  enum {
    TranslationRole = Qt::UserRole,
    LanguageRole = Qt::UserRole + 1,
  };

public:
  Translations(QObject *parent = 0);
  ~Translations();

  int installedCount() const;

  Settings *settings() const;
  void setSettings(Settings *settings);

  Downloader *downloader() const;
  void setDownloader(Downloader *downloader);

  DataProvider *data() const;
  void setData(DataProvider *data);

  Q_INVOKABLE bool loadTranslation(const QString& id);
  Q_INVOKABLE bool removeTranslation(const QString& id);

  QString indexPath(int tid) const;
  QString dataPath(int tid) const;

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
  void refresh();

signals:
  void installedCountChanged();
  void settingsChanged();
  void downloaderChanged();
  void dataChanged();
  void refreshed();
  void downloadError(const QString& name);

private slots:
  void translationStatusChanged();

private:
  int lookup(const QString& id);
  QString translationId(int tid) const;
  void clear();

  QList<Translation *> m_translations;
  Downloader *m_downloader;

  Settings *m_settings;
  DataProvider *m_data;

#ifdef QT_VERSION_5
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* TRANSLATIONS_H */
