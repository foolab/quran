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

#include "chapterinfo.h"
#include "metadata.h"

ChapterInfo::ChapterInfo(int index) {
  init(index);
}

ChapterInfo::ChapterInfo() :
  m_chapter(0) {

}

ChapterInfo::~ChapterInfo() {
  m_chapter = 0;
}

void ChapterInfo::init(int index) {
  m_chapter = chapter(index);
}

int ChapterInfo::offset() const {
  return m_chapter ? m_chapter->offset : -1;
}

int ChapterInfo::length() const {
  return m_chapter ? m_chapter->length : -1;
}

int ChapterInfo::page() const {
  return m_chapter ? m_chapter->page : -1;
}

int ChapterInfo::index() const {
  return m_chapter ? m_chapter->index : -1;
}

QString ChapterInfo::name() const {
  return m_chapter ? QString::fromUtf8(m_chapter->name) : QString();
}

QString ChapterInfo::fullName() const {
  return m_chapter ? QString("%1 %2").arg(QString::fromUtf8(CHAPTER)).arg(name()) : QString();
}

QString ChapterInfo::translatedName() const {
  return m_chapter ? QString::fromUtf8(m_chapter->translation) : QString();
}

QString ChapterInfo::transliteratedName() const {
  return m_chapter ? QString::fromUtf8(m_chapter->transliteration) : QString();
}

bool ChapterInfo::hasBasmala() const {
  if (index() == 0 || index() == 8) {
    return false;
  }

  return true;
}

DeclarativeChapterInfo::DeclarativeChapterInfo(QObject *parent) :
  QObject(parent) {

}

int DeclarativeChapterInfo::chapter() const {
  return index();
}

void DeclarativeChapterInfo::setChapter(int chapter) {
  if (index() != chapter) {
    init(chapter);
    emit chapterChanged();
  }
}
