/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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

#ifndef FAVORITES_DIALOG_H
#define FAVORITES_DIALOG_H

#include <QDialog>
#include <QVariant>

class Bookmarks;
class DataProvider;
class Settings;
class QTreeWidget;

class FavoritesDialog : public QDialog {
  Q_OBJECT

public:
  FavoritesDialog(Settings *settings, Bookmarks *bookmarks, DataProvider *data,
		  QWidget *parent = 0);
  ~FavoritesDialog();

  int sura() const;
  int aya() const;

signals:
  void showBanner(const QString& message);

private slots:
  void go();
  void clear();
  void remove();

private:
  void showEmpty();
  void showFavorites(const QVariantList& bs);

  Settings *m_settings;
  DataProvider *m_data;
  Bookmarks *m_bookmarks;

  QTreeWidget *m_widget;
  int m_sura;
  int m_aya;
};

#endif /* FAVORITES_DIALOG_H */
