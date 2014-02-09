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

#ifndef QURAN_VIEW_MODEL2_H
#define QURAN_VIEW_MODEL2_H

#include <QAbstractListModel>
#include <QMultiMap>

class DataProvider;
class QuranViewModelData;

class QuranViewModel2 : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int page READ page WRITE setPage);
  Q_PROPERTY(DataProvider * data READ data WRITE setData);
  Q_ENUMS(Type);

public:
  QuranViewModel2(QObject *parent = 0);
  ~QuranViewModel2();

  typedef enum {
    ChapterRole = Qt::UserRole + 1,
    VerseRole,
    TypeRole,
  } Roles;

  typedef enum {
    Title,
    Verse,
  } Type;

  void setData(DataProvider *data);
  DataProvider *data() const;

  void setPage(int page);
  int page() const;

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Q_INVOKABLE int findIndex(int chapter, int verse);

private:
  void populate();
  void clear();

  class Info {
  public:
  Info(int chapter, int verse, QuranViewModel2::Type type) :
    m_chapter(chapter), m_verse(verse), m_type(type) {}

    int m_chapter;
    int m_verse;
    QuranViewModel2::Type m_type;
  };

  int m_page;
  DataProvider *m_data;

  QList<Info> m_info;
};

#endif /* QURAN_VIEW_MODEL2_H */
