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

#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <QList>
#include <QStringList>
#include <QObject>

class Page;
class Fragment;
class TextProvider;
class ChapterInfo;

class DataProvider : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString basmala READ basmala NOTIFY basmalaChanged);
  Q_PROPERTY(int pageCount READ pageCount CONSTANT);
  Q_PROPERTY(int partCount READ partCount CONSTANT);
  Q_PROPERTY(int chapterCount READ chapterCount CONSTANT);

public:
  DataProvider(QObject *parent = 0);
  ~DataProvider();

  int pageCount() const;
  int partCount() const;
  Q_INVOKABLE QStringList surasForPage(int page);
  int chapterCount() const;
  Q_INVOKABLE int pageNumberForSuraAndAya(int sura, int aya);
  Q_INVOKABLE QString partNameForPage(int page);
  Q_INVOKABLE QString partName(int part);

  Q_INVOKABLE int firstSuraForPage(int page);

  Q_INVOKABLE bool hasPage(int page) const;

  QStringList availableTexts() const;
  Q_INVOKABLE bool setTextType(int index);
  Q_INVOKABLE int textType() const;

  Page pageFromIndex(int index) const;

  Page pageForSura(int sura) const;

  Page page(int sura, int aya) const;

  QString basmala() const;

  QString prefix() const;

  QString text(int sura, int aya) const;

  QStringList text(const Fragment& frag) const;
  QStringList secondaryText(const Fragment& frag) const;
  QString secondaryText(int sura, int aya);

  void setSecondaryText(TextProvider *text);

  TextProvider *secondaryTextProvider() const;

  Q_INVOKABLE int pageNumberForPart(int part);
  Q_INVOKABLE int partNumberForPage(int page);

  Q_INVOKABLE int suraSize(int sura);

  QList<Fragment> fragmentsForPart(int part);

signals:
  void basmalaChanged();
  void textTypeChanged();
  void secondaryTextProviderChanged();

private:
  int m_index;
  TextProvider *m_data;
  TextProvider *m_secondary;
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

  int m_index;
};

#endif /* DATA_PROVIDER_H */
