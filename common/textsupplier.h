/*
 * Copyright (c) 2014 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef TEXT_SUPPLIER_H
#define TEXT_SUPPLIER_H

#include <QObject>

class DataProvider;

class TextSupplier : public QObject {
  Q_OBJECT
  Q_PROPERTY(DataProvider * data READ data WRITE setData NOTIFY dataChanged);
  Q_PROPERTY(int chapter READ chapter WRITE setChapter NOTIFY chapterChanged);
  Q_PROPERTY(int verse READ verse WRITE setVerse NOTIFY verseChanged);
  Q_PROPERTY(QString primaryText READ primaryText NOTIFY primaryTextChanged);
  Q_PROPERTY(QString secondaryText READ secondaryText NOTIFY secondaryTextChanged);

public:
  TextSupplier(QObject *parent = 0);
  ~TextSupplier();

  void setData(DataProvider *data);
  DataProvider *data() const;

  int chapter() const;
  void setChapter(int chapter);

  int verse() const;
  void setVerse(int verse);

  QString primaryText() const;
  QString secondaryText() const;

signals:
  void dataChanged();
  void verseChanged();
  void chapterChanged();
  void primaryTextChanged();
  void secondaryTextChanged();

private slots:
  void recheck();
  void recheckPrimary();
  void recheckSecondary();

private:
  DataProvider *m_data;
  int m_chapter;
  int m_verse;
};

#endif /* TEXT_SUPPLIER_H */
