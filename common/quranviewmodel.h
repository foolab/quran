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

#ifndef QURAN_VIEW_MODEL_H
#define QURAN_VIEW_MODEL_H

#include <QObject>
#include <QMultiMap>

class DataProvider;
class QuranViewModelData;

class QuranViewModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(int page READ page WRITE setPage);
  Q_PROPERTY(DataProvider * data READ data WRITE setData);

public:
  QuranViewModel(QObject *parent = 0);
  ~QuranViewModel();

  void setData(DataProvider *data);
  DataProvider *data() const;

  void setPage(int page);
  int page() const;

  Q_INVOKABLE QList<int> chapters();
  Q_INVOKABLE QList<int> verses(int chapter);

public slots:
  void populate();

private:
  int m_page;
  DataProvider *m_data;

  QMultiMap<int, int> m_frags;
};

#endif /* QURAN_VIEW_MODEL_H */
