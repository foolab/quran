/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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
#include <deque>

class Downloader;
class DataProvider;
class Translation;

class Translations : public QObject {
  Q_OBJECT

  Q_PROPERTY(int installedCount READ installedCount NOTIFY installedCountChanged);
  Q_PROPERTY(QString dir READ dir WRITE setDir NOTIFY dirChanged);
  Q_PROPERTY(DataProvider *data READ data WRITE setData NOTIFY dataChanged);

public:
  Translations(QObject *parent = 0);
  ~Translations();

  int installedCount() const;

  QString dir() const;
  void setDir(const QString& dir);

  DataProvider *data() const;
  void setData(DataProvider *data);

  Q_INVOKABLE QString findInstalledTranslation();
  Q_INVOKABLE bool loadTranslation(const QString& id);
  Q_INVOKABLE bool removeTranslation(const QString& id);

  QString indexPath(const QString& id) const;
  QString dataPath(const QString& id) const;

  int count();
  Translation *translation(int index);

public slots:
  void refresh();

signals:
  void installedCountChanged();
  void dirChanged();
  void dataChanged();
  void refreshed();
  void downloadError(const QString& name);
  void translationInstalled(Translation *t);
  void translationRemoved(Translation *t);

private:
  Translation *lookup(const QString& id);
  void clear();
  bool isInstalled(Translation *t);
  void ensureDir() const;

  std::deque<Translation *> m_translations;
  Downloader *m_downloader;

  QString m_dir;
  DataProvider *m_data;
};

#endif /* TRANSLATIONS_H */
