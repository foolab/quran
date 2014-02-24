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

#include "quranviewmodel2.h"
#include "dataprovider.h"
#include <QDebug>

QuranViewModel2::QuranViewModel2(QObject *parent)
  : QAbstractListModel(parent),
    m_page(-1),
    m_data(0) {

  QHash<int, QByteArray> roles;
  roles[ChapterRole] = "chapter";
  roles[VerseRole] = "verse";
  roles[TypeRole] = "type";

  setRoleNames(roles);
}

QuranViewModel2::~QuranViewModel2() {

}

void QuranViewModel2::setData(DataProvider *data) {
  if (m_data != data) {
    m_data = data;
  }

  if (m_data && m_page != -1) {
    populate();
  }
}

DataProvider *QuranViewModel2::data() const {
  return m_data;
}

void QuranViewModel2::setPage(int page) {
  if (m_page != page) {
    m_page = page;
  }

  if (m_data && m_page != -1) {
    populate();
  }
}

int QuranViewModel2::page() const {
  return m_page;
}

int QuranViewModel2::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return m_info.size();
  }

  return 0;
}

QVariant QuranViewModel2::data(const QModelIndex& index, int role) const {
  int row = index.row();

  if (row >= m_info.size()) {
    return QVariant();
  }

  switch (role) {
  case ChapterRole:
    return m_info[row].m_chapter;

  case VerseRole:
    return m_info[row].m_verse;

  case TypeRole:
    return m_info[row].m_type;

  default:
    break;
  }

  return QVariant();
}

void QuranViewModel2::populate() {
  clear();

  QList<Info> info;

  QList<Fragment> frags = m_data->pageFromIndex(m_page).fragments();
  foreach(const Fragment& frag, frags) {
    for (int x = frag.start(); x < frag.start() + frag.size(); x++) {
      if (x == 0) {
	// Each chapter has a title.
	// The delegate will take care of showing it.
	info << Info(frag.sura(), -1, Title);
      }

      info << Info(frag.sura(), x, Verse);
    }
  }

  if (!info.isEmpty()) {
    beginInsertRows(QModelIndex(), 0, info.size() - 1);
    m_info = info;
    endInsertRows();
  }
}

void QuranViewModel2::clear() {
  if (!m_info.isEmpty()) {
    beginRemoveRows(QModelIndex(), 0, m_info.size() - 1);
    m_info.clear();
    endRemoveRows();
  }
}

int QuranViewModel2::findIndex(int chapter, int verse) {
  for (int x = 0; x < m_info.size(); x++) {
    const Info& info = m_info[x];
    if (info.m_chapter == chapter && info.m_verse == verse && info.m_type == Verse) {
      return x;
    }
  }

  return -1;
}

#ifdef SAILFISH
QHash<int, QByteArray> QuranViewModel2::roleNames() const {
  return m_roles;
}

void QuranViewModel2::setRoleNames(const QHash<int, QByteArray>& roles) {
  m_roles = roles;
}
#endif
