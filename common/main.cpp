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
#include "aboutdata.h"
#include <QDir>
#include "windowcontroller.h"
#include "translations.h"
#include "translation.h"
#include "downloader.h"
#include "fsmonitor.h"
#include "label.h"
#include "search.h"
#include "recitations.h"
#include "gstzipsrc.h"
#include "colors.h"

#include <MDeclarativeCache>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MPannableViewport>
#include <MPositionIndicator>

#define USER_DIR "/home/user/MyDocs/.n9-quran/"

Q_DECL_EXPORT int main(int argc, char *argv[]) {
  MApplication *app = new MApplication(argc, argv);

  gst_init(0, 0);
  gst_zip_src_register();

  bool dev = false;
  for (int x = 0; x < argc; x++) {
    if (QLatin1String("-quran-dev") == QLatin1String(argv[x])) {
      dev = true;
      break;
    }
  }

  FSMonitor monitor;

  Settings settings;
  settings.loadFont();

  Downloader downloader;

  DataProvider data;

  Translations translations(USER_DIR "translations/", &downloader, &settings, &data);
  Recitations recitations(USER_DIR "recitations/", &downloader, &settings, &data);

  Bookmarks bookmarks(&settings);

  NumberFormatter formatter(&settings);

  AboutData about;

  Search search(DATA_DIR "/search.db");

  Colors c(DATA_DIR "themes", &settings);

  // TODO: Is all this needed ?
  qmlRegisterType<DataProvider>();
  qmlRegisterType<Settings>();
  qmlRegisterType<Bookmarks>();
  qmlRegisterType<NumberFormatter>();
  qmlRegisterType<AboutData>();
  qmlRegisterType<Translations>();
  qmlRegisterType<FSMonitor>();
  qmlRegisterType<Search>();
  qmlRegisterType<Translation>("Translations", 1, 0, "Translation");
  qmlRegisterType<QuranViewModel>("QuranViewModel", 1, 0, "QuranViewModel");
  qmlRegisterType<Label>("Label2", 1, 0, "Label2");

  MApplicationWindow *view = new MApplicationWindow;
  view->setRoundedCornersEnabled(false);
  MApplicationPage *page = new MApplicationPage();
  page->setPannable(false);
  page->appear();
  page->pannableViewport()->positionIndicator()->hide();
  QDeclarativeEngine *engine = new QDeclarativeEngine;

  QDeclarativeContext *rootContext = engine->rootContext();

  view->setWindowTitle(QObject::tr("Holy Quran"));

  QObject::connect(engine, SIGNAL(quit()), app, SLOT(quit()));

  ThemeImageProvider *theme = new ThemeImageProvider(DATA_DIR "/themes/");

  engine->addImageProvider("quran", new LogoProvider);
  engine->addImageProvider("theme", theme);

  rootContext->setContextProperty("_settings", &settings);
  rootContext->setContextProperty("_data", &data);
  rootContext->setContextProperty("_bookmarks", &bookmarks);
  rootContext->setContextProperty("_formatter", &formatter);
  rootContext->setContextProperty("_about", &about);
  rootContext->setContextProperty("_translations", &translations);
  rootContext->setContextProperty("_downloader", &downloader);
  rootContext->setContextProperty("_fsmon", &monitor);
  rootContext->setContextProperty("_theme", theme);
  rootContext->setContextProperty("_search", &search);
  rootContext->setContextProperty("_recitations", &recitations);
  rootContext->setContextProperty("_colors", &c);

  QUrl sourceUrl = dev ? QUrl::fromLocalFile(QDir::currentPath() + "/main.qml")
    : QUrl("qrc:/qml/main.qml");
  if (!dev) {
    engine->addImportPath(DATA_DIR "/qml");
  }

  QDeclarativeItem *root = 0;

  QDeclarativeComponent component(engine, sourceUrl);
  QGraphicsObject *content = qobject_cast<QGraphicsObject *>(component.create());

  root = qobject_cast<QDeclarativeItem *>(content);

  MWidget *centralWidget = new MWidget;
  content->setParentItem(centralWidget);
  centralWidget->setMinimumSize(864, 400);
  page->setCentralWidget(centralWidget);

  WindowController controller(view, &settings, root);

  controller.exposedContentRectChanged();

  controller.setOrientation();
  controller.show();

  int ret = app->exec();

  delete view;
  delete app;

  return ret;
}
