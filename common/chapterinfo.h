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

#ifndef CHAPTER_INFO_H
#define CHAPTER_INFO_H

#include <QObject>
#include <QString>

struct Chapter;

class ChapterInfo {
public:
  ChapterInfo();
  ChapterInfo(int index);
  virtual ~ChapterInfo();

  int offset() const;
  int length() const;
  int page() const;
  int index() const;

  QString name() const;
  QString fullName() const;
  QString translatedName() const;
  QString transliteratedName() const;

  bool hasBasmala() const;

protected:
  void init(int index);
  Chapter *m_chapter;
};

class DeclarativeChapterInfo : public QObject, public ChapterInfo {
  Q_OBJECT
  Q_PROPERTY(int chapter READ chapter WRITE setChapter NOTIFY chapterChanged);
  Q_PROPERTY(int page READ page NOTIFY chapterChanged);
  Q_PROPERTY(int length READ length NOTIFY chapterChanged);
  Q_PROPERTY(int offset READ offset NOTIFY chapterChanged);
  Q_PROPERTY(QString name READ name NOTIFY chapterChanged);
  Q_PROPERTY(QString fullName READ fullName NOTIFY chapterChanged);
  Q_PROPERTY(QString translatedName READ translatedName NOTIFY chapterChanged);
  Q_PROPERTY(QString transliteratedName READ transliteratedName NOTIFY chapterChanged);
  Q_PROPERTY(bool hasBasmala READ hasBasmala NOTIFY chapterChanged);

public:
  DeclarativeChapterInfo(QObject *parent = 0);

  int chapter() const;
  void setChapter(int chapter);

signals:
  void chapterChanged();
};

#endif /* CHAPTER_INFO_H */
