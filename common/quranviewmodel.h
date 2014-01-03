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

#ifndef QURAN_VIEW_MODEL_H
#define QURAN_VIEW_MODEL_H

#include <QAbstractListModel>
#include <QMultiMap>

class DataProvider;
class QuranViewModelData;

class QuranViewModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int page READ page WRITE setPage);
  Q_PROPERTY(DataProvider * data READ data WRITE setData);

public:
  QuranViewModel(QObject *parent = 0);
  ~QuranViewModel();

  typedef enum {
    ChapterRole = Qt::UserRole,
    VerseRole = Qt::UserRole + 1,
  } Roles;

  void setData(DataProvider *data);
  DataProvider *data() const;

  void setPage(int page);
  int page() const;

  Q_INVOKABLE QList<int> chapters();
  Q_INVOKABLE QList<int> verses(int chapter);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
  void populate();

private:
  class Info {
  public:
    Info(int chapter, int verse) :
      m_chapter(chapter), m_verse(verse) {}

    int m_chapter;
    int m_verse;
  };

  int m_page;
  DataProvider *m_data;

  QMultiMap<int, int> m_frags;
  QList<Info> m_items;

#ifdef SAILFISH
  QHash<int, QByteArray> roleNames() const;
  void setRoleNames(const QHash<int, QByteArray>& roles);
  QHash<int, QByteArray> m_roles;
#endif
};

#endif /* QURAN_VIEW_MODEL_H */
