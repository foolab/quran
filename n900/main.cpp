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
#include <QtDeclarative>
#include "settings.h"
#include "dataprovider.h"
#include "quranviewmodel.h"
#include "bookmarks.h"
#include "numberformatter.h"
#include "logoprovider.h"
#include "themeimageprovider.h"
#include "legal.h"
#include <QDir>
#include "windowcontroller.h"
#include "translations.h"
#include "translation.h"
#include "downloader.h"
#include "fsmonitor.h"

#ifndef Q_WS_MAEMO_5
#include <MDeclarativeCache>
#else
#define M_DECL_EXPORT
#endif

#define USER_DIR "/home/user/MyDocs/.n900-quran/"

M_DECL_EXPORT int main(int argc, char *argv[]) {
#ifndef Q_WS_MAEMO_5
  QApplication *app = MDeclarativeCache::qApplication(argc, argv);
  app->setProperty("NoMStyle", true);
#else
  // PR 1.3 Qt hildon style will crash when we are launched in portrait mode.
  // We don't use Qt so we choose an arbitrary style.
  // TODO: seems the only way is to pass -style
  QApplication::setStyle("clearlooks");
  QApplication *app = new QApplication(argc, argv);
#endif

  bool dev = false;
  for (int x = 0; x < argc; x++) {
    if (QLatin1String("-dev") == QLatin1String(argv[x])) {
      dev = true;
      break;
    }
  }

  FSMonitor monitor;

  Settings settings;
  settings.loadFont();

  Downloader downloader;

  DataProvider data(DATA_DIR "/text/");

  Translations translations(USER_DIR "translations/", &downloader, &settings, &data);

  Bookmarks bookmarks(&settings);

  NumberFormatter formatter(&settings);

  Legal legal;

  qmlRegisterType<DataProvider>();
  qmlRegisterType<Settings>();
  qmlRegisterType<Bookmarks>();
  qmlRegisterType<NumberFormatter>();
  qmlRegisterType<Legal>();
  qmlRegisterType<Translations>();
  qmlRegisterType<FSMonitor>();
  qmlRegisterType<Translation>("Translations", 1, 0, "Translation");
  qmlRegisterType<QuranViewModel>("QuranViewModel", 1, 0, "QuranViewModel");


#ifndef Q_WS_MAEMO_5
  QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();
#else
  QDeclarativeView *view = new QDeclarativeView;
  view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
#endif

  view->setWindowTitle(QObject::tr("Holy Quran"));

  QObject::connect(view->engine(), SIGNAL(quit()), app, SLOT(quit()));

  ThemeImageProvider *theme = new ThemeImageProvider(DATA_DIR "/themes/");

  view->engine()->addImageProvider("quran", new LogoProvider);
  view->engine()->addImageProvider("theme", theme);

  view->rootContext()->setContextProperty("_settings", &settings);
  view->rootContext()->setContextProperty("_data", &data);
  view->rootContext()->setContextProperty("_bookmarks", &bookmarks);
  view->rootContext()->setContextProperty("_formatter", &formatter);
  view->rootContext()->setContextProperty("_legal", &legal);
  view->rootContext()->setContextProperty("_translations", &translations);
  view->rootContext()->setContextProperty("_downloader", &downloader);
  view->rootContext()->setContextProperty("_fsmon", &monitor);
  view->rootContext()->setContextProperty("_theme", theme);

  if (dev) {
    view->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/main.qml"));
  }
  else {
    view->engine()->addImportPath(DATA_DIR "/qml");
    view->setSource(QUrl::fromLocalFile(DATA_DIR "/qml/" "main.qml"));
  }

  WindowController controller(view, &settings);

#ifdef Q_WS_MAEMO_5
  controller.setOrientation();
#endif

  controller.show();

  /*
#ifndef Q_WS_MAEMO_5
  view->showFullScreen();
#else
  view->showMaximized();
#endif
  */

  int ret = app->exec();

  delete view;

#ifndef Q_WS_MAEMO_5
  // We get a crash deep in the stack from Qt
  // We will not delete QApplication until I debug that properly
  delete app;
#endif

  return ret;
}
