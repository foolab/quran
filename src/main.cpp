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
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include "settings.h"
#include "dataprovider.h"
#include "quranview.h"
#include "bookmarks.h"
#include "numberformatter.h"
#include "pagepositioncontroller.h"
#include "imageprovider.h"
#include <QDir>
#include <MDeclarativeCache>

Q_DECL_EXPORT int main(int argc, char *argv[]) {
  QApplication *app = MDeclarativeCache::qApplication(argc, argv);
  app->setProperty("NoMStyle", true);

  bool dev = false;
  for (int x = 0; x < argc; x++) {
    if (QLatin1String("-dev") == QLatin1String(argv[x])) {
      dev = true;
      break;
    }
  }

  Settings settings;
  settings.loadFont();

  DataProvider data(DATA_DIR "/text/");

  Bookmarks bookmarks(&settings);

  NumberFormatter formatter(&settings);

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
}
