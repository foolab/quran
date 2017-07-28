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

#ifndef PAGE_POSITION_H
#define PAGE_POSITION_H

#include <QObject>

class PagePosition : public QObject {
  Q_OBJECT
  Q_PROPERTY(int verse READ verse WRITE setVerse NOTIFY verseChanged);
  Q_PROPERTY(int chapter READ chapter WRITE setChapter NOTIFY chapterChanged);
  Q_PROPERTY(int page READ page NOTIFY pageChanged);
  Q_PROPERTY(bool isValid READ isValid NOTIFY pageChanged);

public:
  PagePosition(QObject *parent = 0);
  ~PagePosition();

  int verse() const;
  void setVerse(int verse);

  int chapter() const;
  void setChapter(int chapter);

  int page() const;
  void setPage(int page);

  bool isValid() const;

public slots:
  void reset();

signals:
  void chapterChanged();
  void verseChanged();
  void pageChanged();

private:
  void resetPage();

  int m_chapter;
  int m_verse;
  int m_page;
};

#endif /* PAGE_POSITION_H */
