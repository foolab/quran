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

  // TODO: fix this mess!
  if (!settings.isFontLoaded()) {
    QMaemo5InformationBox::information(&win, QObject::tr("Failed to load application font"));
    win.createContent();
  }
  else if (!data.setText(settings.textType())) {
    win.show();
    QMaemo5InformationBox::information(&win,
    QObject::tr("Failed to load the Quran text. Please reinstall"),
				       QMaemo5InformationBox::NoTimeout);
  }
  else {
    win.createContent();
    QTimer::singleShot(0, &win, SLOT(load()));
  }

  win.show();

  return app.exec();

#if 0


  PagePositionController position;

  qmlRegisterType<DataProvider>();
  qmlRegisterType<Settings>();
  qmlRegisterType<Bookmarks>();
  qmlRegisterType<NumberFormatter>();
  qmlRegisterType<PagePositionController>();
  qmlRegisterType<QuranView>("Quran", 1, 0, "QuranView");

  QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();

  view->engine()->addImageProvider("quran", new ImageProvider);

  view->rootContext()->setContextProperty("_settings", &settings);
  view->rootContext()->setContextProperty("_data", &data);
  view->rootContext()->setContextProperty("_bookmarks", &bookmarks);
  view->rootContext()->setContextProperty("_formatter", &formatter);
  view->rootContext()->setContextProperty("_position", &position);

  if (dev) {
    view->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/main.qml"));
  }
  else {
    view->engine()->addImportPath(DATA_DIR "/qml");
    view->setSource(QUrl::fromLocalFile(DATA_DIR "/qml/" "main.qml"));
  }

  view->showFullScreen();

  return app->exec();
#endif
}
