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

#include "pageposition.h"
#include "chapterinfo.h"
#include "pageinfo.h"
#include "metadata.h"

PagePosition::PagePosition(QObject *parent) :
  QObject(parent),
  m_chapter(-1),
  m_verse(-1),
  m_page(-1) {

}

PagePosition::~PagePosition() {

}

int PagePosition::verse() const {
  return m_verse;
}

void PagePosition::setVerse(int verse) {
  if (m_verse != verse) {
    m_verse = verse;
    emit verseChanged();
  }

  resetPage();
}

int PagePosition::chapter() const {
  return m_chapter;
}

void PagePosition::setChapter(int chapter) {
  if (m_chapter != chapter) {
    m_chapter = chapter;
    emit chapterChanged();
  }

  resetPage();
}

int PagePosition::page() const {
  return m_page;
}

void PagePosition::setPage(int page) {
  if (m_page != page) {
    m_page = page;
    emit pageChanged();
  }
}

bool PagePosition::isValid() const {
  return m_page != -1;
}

void PagePosition::reset() {
  setChapter(-1);
  setVerse(-1);
}

void PagePosition::resetPage() {
  if (m_chapter == -1 || m_verse == -1) {
    setPage(-1);
    return;
  }

  for (int x = ChapterInfo(m_chapter).page(); x < MAX_PAGE + 1; x++) {
    for (const FragmentInfo& info : PageInfo(x).fragments()) {
      if (info.contains(m_chapter, m_verse)) {
	setPage(x);
	return;
      }
    }
  }

  setPage(-1);
}
