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
#include "numberformatter.h"
#include "dataprovider.h"
#include "quranview.h"
#include <QDebug>
#include <QPushButton>
#include <QMenuBar>
#include <QScrollBar>
#include <QMaemo5InformationBox>
#include "settingsdialog.h"
#include "indexdialog.h"
#include "aboutdialog.h"
#include "bookmarks.h"
#include "favoritesdialog.h"

MainWindow::MainWindow(Settings *settings, Bookmarks *bookmarks,
		       DataProvider *data, NumberFormatter *formatter,
		       QWidget *parent)
  : QMainWindow(parent), m_settings(settings), m_bookmarks(bookmarks),
    m_data(data), m_formatter(formatter), m_part(0), m_sura(0), m_page(0),
    m_view(0), m_forward(0), m_back(0) {

  setWindowTitle(tr("Holy Quran"));
  setAttribute(Qt::WA_Maemo5PortraitOrientation);

  QObject::connect(m_settings, SIGNAL(fontSizeChanged()), this, SLOT(fontSizeChanged()));
  QObject::connect(m_settings, SIGNAL(textTypeChanged()), this, SLOT(textTypeChanged()));
  QObject::connect(m_settings, SIGNAL(numberFormatChanged()), this, SLOT(numberFormatChanged()));
  QObject::connect(m_settings, SIGNAL(pageNumberChanged()), this, SLOT(pageNumberChanged()));
  QObject::connect(m_settings, SIGNAL(yChanged()), this, SLOT(yChanged()));
  QObject::connect(m_bookmarks, SIGNAL(bookmarkAdded(uint, int)), this, SLOT(bookmarkAdded()));
  QObject::connect(m_bookmarks, SIGNAL(bookmarkRemoved(uint, int)), this, SLOT(bookmarkRemoved()));
  QObject::connect(m_bookmarks, SIGNAL(cleared()), this, SLOT(bookmarksCleared()));
}

MainWindow::~MainWindow() {

}

void MainWindow::createErrorContent() {
  QLabel *label = new QLabel(tr("Failed to load the Quran text. Please reinstall."), this);
  label->setWordWrap(true);
  label->setAlignment(Qt::AlignCenter);
  setCentralWidget(label);
}

void MainWindow::createContent() {
  QWidget *widget = new QWidget(this);
  setCentralWidget(widget);

  m_part = new QLabel(this);
  m_part->setAlignment(Qt::AlignRight);

  m_sura = new QLabel(this);
  m_sura->setAlignment(Qt::AlignLeft);

  m_page = new QLabel(this);
  m_page->setAlignment(Qt::AlignCenter);

  m_forward = new QPushButton(QIcon::fromTheme("general_back"), QString(), this);
  m_back = new QPushButton(QIcon::fromTheme("general_forward"), QString(), this);

  QGridLayout *layout = new QGridLayout(widget);
  layout->addWidget(m_sura, 0, 0, 1, 1);
  layout->addWidget(m_part, 0, 2, 1, 1);

  m_view = new QuranView(m_settings, this);

  layout->addWidget(m_view, 1, 0, 1, 3);

  m_view->setDataProvider(m_data);
  m_view->setBookmarks(m_bookmarks);
  m_view->setFormatter(m_formatter);

  fontSizeChanged();

  m_view->setHighlightColor(m_settings->highlightColor());

  QObject::connect(m_forward, SIGNAL(clicked()), this, SLOT(nextPage()));
  QObject::connect(m_back, SIGNAL(clicked()), this, SLOT(previousPage()));

  {
    QWidget *w = new QWidget(this);
    QHBoxLayout *l = new QHBoxLayout(w);
    layout->addWidget(w, 2, 0, 1, 3);
    l->addWidget(m_forward);
    l->addWidget(m_page);
    l->addWidget(m_back);
  }

  menuBar()->addAction(tr("Settings"), this, SLOT(showSettings()));
  menuBar()->addAction(tr("Index"), this, SLOT(showIndex()));
  menuBar()->addAction(tr("Favorites"), this, SLOT(showFavorites()));
  menuBar()->addAction(tr("About"), this, SLOT(showAbout()));

  QScrollBar *bar = m_view->verticalScrollBar();
  QObject::connect(bar, SIGNAL(valueChanged(int)), m_settings, SLOT(setY(int)));
}

void MainWindow::setPage(int page) {
  m_view->populate(page);
  m_settings->setY(0);

  m_forward->setEnabled(m_data->hasPage(page + 1));
  m_back->setEnabled(m_data->hasPage(page - 1));
}

void MainWindow::load() {
  int y = m_settings->y();

  // Validate page number...
  int n = m_settings->pageNumber();
  qMin(qMax(n, 0), m_data->pageCount());

  if (n != m_settings->pageNumber()) {
    m_settings->setPageNumber(n);
  }
  else {
    pageNumberChanged();
  }

  // pageNumberChanged() will reset it.
  m_settings->setY(y);
}

void MainWindow::fontSizeChanged() {
  AbstractQuranView *view = dynamic_cast<AbstractQuranView *>(m_view);
  view->setFont(QFont(m_settings->fontFamily(), m_settings->fontSize()));
}

void MainWindow::textTypeChanged() {
  setPage(m_settings->pageNumber());
}

void MainWindow::numberFormatChanged() {
  pageNumberChanged();
}

void MainWindow::pageNumberChanged() {
  setPage(m_settings->pageNumber());
  m_page->setText(m_formatter->number(m_settings->pageNumber() + 1));
  m_part->setText(m_data->partName(m_settings->pageNumber()));
  m_sura->setText(m_data->surasForPage(m_settings->pageNumber()).join("  "));
}

void MainWindow::yChanged() {
  m_view->setY(m_settings->y());
}

void MainWindow::nextPage() {
  if (m_settings->pageNumber() == m_data->pageCount() - 1) {
    QMaemo5InformationBox::information(this, tr("You are at the last page"));
  }
  else {
    m_settings->setPageNumber(m_settings->pageNumber() + 1);
  }
}

void MainWindow::previousPage() {
  if (m_settings->pageNumber() == 0) {
    QMaemo5InformationBox::information(this, tr("You are at the first page"));
  }
  else {
    m_settings->setPageNumber(m_settings->pageNumber() - 1);
  }
}

void MainWindow::showSettings() {
  SettingsDialog dlg(m_settings, m_data, m_formatter);
  dlg.exec();
}

void MainWindow::showIndex() {
  IndexDialog dlg(m_settings, m_data, m_formatter);
  if (dlg.exec() == QDialog::Accepted) {
    int sura = dlg.sura();
    m_settings->setPageNumber(m_data->pageNumberForSura(sura));
    m_view->scrollToPosition(Position(sura));
  }
}

void MainWindow::showFavorites() {
  FavoritesDialog dlg(m_settings, m_bookmarks, m_data);
  if (dlg.exec() != QDialog::Accepted) {
    return;
  }

  Position pos(dlg.sura(), dlg.aya());
  if (!pos.isValid()) {
    return;
  }

  m_settings->setPageNumber(m_data->pageNumberForSuraAndAya(pos.sura(), pos.aya()));
  m_view->scrollToPosition(pos);
}

void MainWindow::showAbout() {
  AboutDialog dlg(m_settings);
  dlg.exec();
}

void MainWindow::bookmarkAdded() {
  QMaemo5InformationBox::information(this, tr("Favorite added"));
}

void MainWindow::bookmarkRemoved() {
  QMaemo5InformationBox::information(this, tr("Favorite removed"));
}

void MainWindow::bookmarksCleared() {
  QMaemo5InformationBox::information(this, tr("Favorites cleared"));
}
