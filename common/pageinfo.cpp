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

#include "pageinfo.h"
#include "metadata.h"
#include "chapterinfo.h"

PageInfo::PageInfo(int index) {
  init(index);
}

PageInfo::PageInfo() :
  m_page(0) {

}

PageInfo::~PageInfo() {
  m_page = 0;
}

void PageInfo::init(int index) {
  m_page = page(index);
}

int PageInfo::index() const {
  return m_page ? m_page->index : -1;
}

int PageInfo::firstChapter() const {
  return m_page ? m_page->chapter : -1;
}

int PageInfo::firstVerse() const {
  return m_page ? m_page->verse : -1;
}

int PageInfo::lastChapter() const {
  QList<FragmentInfo> info = fragments();
  return info.isEmpty() ? -1 : info.last().chapter();
}

int PageInfo::lastVerse() const {
  QList<FragmentInfo> info = fragments();
  if (info.isEmpty()) {
    return -1;
  }

  FragmentInfo frag = info.last();
  return frag.start() + frag.length() - 1;
}

int PageInfo::firstFragment() const {
  return m_page ? m_page->firstFragment : -1;
}

int PageInfo::numberOfFragments() const {
  return m_page ? m_page->fragments : -1;
}

int PageInfo::part() const {
  return m_page ? m_page->part : -1;
}

QList<FragmentInfo> PageInfo::fragments() const {
  QList<FragmentInfo> info;
  if (m_page) {
    for (int x = m_page->firstFragment; x < m_page->firstFragment + m_page->fragments; x++) {
      info << FragmentInfo(x);
    }
  }

  return info;
}

FragmentInfo::FragmentInfo(int index) {
  m_fragment = fragment(index);
}

int FragmentInfo::start() const {
  return m_fragment ? m_fragment->verse : -1;
}

int FragmentInfo::length() const {
  return m_fragment ? m_fragment->length : -1;
}

int FragmentInfo::chapter() const {
  return m_fragment ? m_fragment->chapter : -1;
}

bool FragmentInfo::contains(int chapter, int verse) const {
  return FragmentInfo::chapter() == chapter
    && start() <= verse
    && start() + length() > verse;
}

QStringList PageInfo::chapters() const {
  QStringList ret;

  QList<FragmentInfo> frags(fragments());

  for (const FragmentInfo& frag : frags) {
    ret << ChapterInfo(frag.chapter()).name();
  }

  return ret;
}

DeclarativePageInfo::DeclarativePageInfo(QObject *parent) :
  QObject(parent) {

}

int DeclarativePageInfo::page() const {
  return index();
}

void DeclarativePageInfo::setPage(int page) {
  if (index() != page) {
    init(page);
    emit pageChanged();
  }
}
