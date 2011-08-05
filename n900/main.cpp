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

#include <QApplication>
#include "mainwindow.h"
#include "settings.h"
#include "dataprovider.h"
#include "bookmarks.h"
#include "numberformatter.h"
#include <QMaemo5InformationBox>
#include <QTimer>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Settings settings;
  settings.loadFont();

  DataProvider data(DATA_DIR "/text/");

  Bookmarks bookmarks(&settings);

  NumberFormatter formatter(&settings);

  MainWindow win(&settings, &bookmarks, &data, &formatter);

  if (!data.setText(settings.textType())) {
    win.createErrorContent();
  }
  else {
    if (!settings.isFontLoaded()) {
      QMaemo5InformationBox::information(&win, QObject::tr("Failed to load application font"));
    }

    win.createContent();
    QTimer::singleShot(0, &win, SLOT(load()));
  }

  win.show();

  return app.exec();
}
