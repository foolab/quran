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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class QLabel;
class Settings;
class Bookmarks;
class DataProvider;
class NumberFormatter;
class QuranView;
class QPushButton;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(Settings *settings, Bookmarks *bookmarks,
	     DataProvider *data, NumberFormatter *formatter,
	     QWidget *parent = 0);
  ~MainWindow();

  void createContent();

  void createErrorContent();

public slots:
  void load();

private slots:
  void fontSizeChanged();
  void textTypeChanged();
  void numberFormatChanged();
  void pageNumberChanged();
  void yChanged();
  void nextPage();
  void previousPage();

  void showSettings();
  void showIndex();
  void showFavorites();
  void showAbout();

  void bookmarkAdded();
  void bookmarkRemoved();
  void bookmarksCleared();

private:
  void setPage(int page);

  Settings *m_settings;
  Bookmarks *m_bookmarks;
  DataProvider *m_data;
  NumberFormatter *m_formatter;

  QLabel *m_part;
  QLabel *m_sura;
  QLabel *m_page;

  QuranView *m_view;
  QPushButton *m_forward;
  QPushButton *m_back;
};

#endif /* MAIN_WINDOW_H */
