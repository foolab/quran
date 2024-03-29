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

#ifndef QURAN_VIEW_MODEL2_H
#define QURAN_VIEW_MODEL2_H

#include <QAbstractListModel>
#include <QMultiMap>

class QuranViewModelData;

class QuranViewModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int page READ page WRITE setPage NOTIFY pageChanged);
  Q_ENUMS(Type);

public:
  QuranViewModel(QObject *parent = 0);
  ~QuranViewModel();

  typedef enum {
    ChapterRole = Qt::UserRole + 1,
    VerseRole,
    TypeRole,
    IsFirstChapterRole,
  } Roles;

  typedef enum {
    Title,
    Verse,
  } Type;

  void setPage(int page);
  int page() const;

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Q_INVOKABLE int findIndex(int chapter, int verse);

signals:
  void pageChanged();
  void dataChanged();

private:
  void populate();
  void clear();

  class Info {
  public:
  Info(int chapter, int verse, QuranViewModel::Type type) :
    m_isFirstChapter(false), m_chapter(chapter), m_verse(verse), m_type(type) {}

    bool m_isFirstChapter;
    int m_chapter;
    int m_verse;
    QuranViewModel::Type m_type;
  };

  QHash<int, QByteArray> roleNames() const;

  int m_page;
  QList<Info> m_info;
};

#endif /* QURAN_VIEW_MODEL2_H */
