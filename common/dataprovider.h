/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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

#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <QList>
#include <QStringList>
#include <QObject>

class Page;
class Fragment;
class Sura;

class DataProvider : public QObject {
  Q_OBJECT

public:
  DataProvider(const QString& dir, QObject *parent = 0);
  ~DataProvider();

  Q_INVOKABLE int pageCount();
  Q_INVOKABLE QStringList surasForPage(int page);
  Q_INVOKABLE int suraCount();
  Q_INVOKABLE QString suraName(int sura);
  Q_INVOKABLE QString fullSuraName(int sura);
  Q_INVOKABLE int pageNumberForSura(int sura);
  Q_INVOKABLE int pageNumberForSuraAndAya(int sura, int aya);
  Q_INVOKABLE QString partName(int page);

  Q_INVOKABLE int firstSuraForPage(int page);

  // TODO:
  //  Q_INVOKABLE int numberOfSuraPages(int sura);

  Q_INVOKABLE bool hasPage(int page) const;

  QStringList availableTexts() const;
  Q_INVOKABLE bool setText(int index);

  Page pageFromIndex(int index) const;

  Page pageForSura(int sura) const;

  Page page(int sura, int aya) const;

  Sura sura(int index) const;

  QString basmala() const;

  QString prefix() const;

  Q_INVOKABLE QString text(int sura, int aya) const;

  QStringList text(const Fragment& frag) const;

private:
  const QString m_dir;

  int m_index;
  char *m_data;
};

class Page {
public:
  int index() const { return m_index; }

  QList<Fragment> fragments();

private:
  friend class DataProvider;

  Page(int index) : m_index(index) {}

  const int m_index;
};

class Fragment {
public:
  int index() const { return m_index; }

  int sura() const;

  int start() const;

  int size() const;

private:
  friend class Page;

  Fragment(int index) : m_index(index) {}

  // TODO: cannot be const
  int m_index;
};

class Sura {
public:
  ~Sura();

  QString name() const;

  bool hasBasmala() const;

  int size() const;

  int index() const;

private:
  friend class DataProvider;

  Sura(int index);

  const int m_index;
};

#endif /* DATA_PROVIDER_H */
