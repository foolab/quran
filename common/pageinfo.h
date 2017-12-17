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

#ifndef PAGE_INFO_H
#define PAGE_INFO_H

#include <QObject>
#include <QStringList>

struct Page;
struct Fragment;
class FragmentInfo;

class PageInfo {
public:
  PageInfo();
  PageInfo(int index);
  virtual ~PageInfo();

  int index() const;
  int firstChapter() const;
  int firstVerse() const;
  int firstFragment() const;
  int lastFragment() const;
  int numberOfFragments() const;
  int part() const;

  QStringList chapters() const;

  QList<FragmentInfo> fragments() const;

protected:
  void init(int index);
  Page *m_page;
};

class FragmentInfo {
public:
  FragmentInfo(int index);

  int start() const;
  int length() const;
  int chapter() const;

  bool contains(int chapter, int verse) const;

private:
  Fragment *m_fragment;
};

class DeclarativePageInfo : public QObject, public PageInfo {
  Q_OBJECT
  Q_PROPERTY(int page READ page WRITE setPage NOTIFY pageChanged);
  Q_PROPERTY(int firstChapter READ firstChapter NOTIFY pageChanged);
  Q_PROPERTY(int part READ part NOTIFY pageChanged);
  Q_PROPERTY(QStringList chapters READ chapters NOTIFY pageChanged);

public:
  DeclarativePageInfo(QObject *parent = 0);

  int page() const;
  void setPage(int page);

signals:
  void pageChanged();
};

#endif /* PAGE_INFO_H */
