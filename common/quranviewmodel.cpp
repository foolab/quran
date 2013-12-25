/*
 * Copyright (c) 2011-2013 Mohammed Sameer <msameer@foolab.org>.
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

#include "quranviewmodel.h"
#include "dataprovider.h"
#include <QDebug>

QuranViewModel::QuranViewModel(QObject *parent)
  : QObject(parent), m_page(-1), m_data(0) {

}

QuranViewModel::~QuranViewModel() {

}

void QuranViewModel::setData(DataProvider *data) {
  if (m_data != data) {
    m_data = data;
  }

  if (m_data && m_page != -1) {
    populate();
  }
}

DataProvider *QuranViewModel::data() const {
  return m_data;
}

void QuranViewModel::setPage(int page) {
  if (m_page != page) {
    m_page = page;
  }

  if (m_data && m_page != -1) {
    populate();
  }
}

int QuranViewModel::page() const {
  return m_page;
}

QList<int> QuranViewModel::chapters() {
  QList<int> c = m_frags.uniqueKeys();

  qSort(c);

  return c;
}

QList<int> QuranViewModel::verses(int chapter) {
  QList<int> v = m_frags.values(chapter);

  qSort(v);

  return v;
}

void QuranViewModel::populate() {
  m_frags.clear();

  QList<Fragment> frags = m_data->pageFromIndex(m_page).fragments();

  foreach(const Fragment& frag, frags) {
    for (int x = frag.start(); x < frag.start() + frag.size(); x++) {
      m_frags.insert(frag.sura(), x);
    }
  }
}
