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

#include "mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include "settings.h"

MainWindow::MainWindow(Settings *settings, Bookmarks *bookmarks,
		       DataProvider *data, NumberFormatter *formatter,
		       QWidget *parent)
  : QMainWindow(parent), m_settings(settings), m_bookmarks(bookmarks),
    m_data(data), m_formatter(formatter), m_part(0), m_verse(0), m_page(0) {

  setWindowTitle(tr("Holy Quran"));
  setAttribute(Qt::WA_Maemo5PortraitOrientation);

  QObject::connect(m_settings, SIGNAL(fontSizeChanged()), this, SLOT(fontSizeChanged()));
  QObject::connect(m_settings, SIGNAL(textTypeChanged()), this, SLOT(textTypeChanged()));
  QObject::connect(m_settings, SIGNAL(numberFormatChanged()), this, SLOT(numberFormatChanged()));
  QObject::connect(m_settings, SIGNAL(pageNumberChanged()), this, SLOT(pageNumberChanged()));
  QObject::connect(m_settings, SIGNAL(positionChanged()), this, SLOT(positionChanged()));
}

MainWindow::~MainWindow() {

}

void MainWindow::createContent() {
  QWidget *widget = new QWidget(this);
  setCentralWidget(widget);

  m_part = new QLabel(this);
  m_verse = new QLabel(this);
  m_page = new QLabel(this);

  QGridLayout *layout = new QGridLayout(widget);
  layout->addWidget(m_verse, 0, 0, 1, 1);
  layout->addWidget(m_part, 0, 2, 1, 1);
  layout->addWidget(m_page, 2, 1, 1, 1);
}

void MainWindow::setPage(int page) {
  
}

void MainWindow::load() {
  pageNumberChanged();
}

void MainWindow::fontSizeChanged() {

}

void MainWindow::textTypeChanged() {

}

void MainWindow::numberFormatChanged() {

}

void MainWindow::pageNumberChanged() {
  setPage(m_settings->pageNumber());
}

void MainWindow::positionChanged() {

}

void MainWindow::nextPage() {

}

void MainWindow::previousPage() {

}
