/*
 * Copyright (c) 2011-2014 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef SAILFISH
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QtDeclarative>
#endif
#include "settings.h"
#include "dataprovider.h"
#include "quranviewmodel.h"
#include "bookmarks.h"
#include "numberformatter.h"
#include "themeimageprovider.h"
#include "aboutdata.h"
#include <QDir>
#ifndef SAILFISH
#include "windowcontroller.h"
#endif
#include "translations.h"
#include "translation.h"
#include "downloader.h"
#include "fsmonitor.h"
#include "search.h"
#include "recitations.h"
#include "colors.h"
#include "toolbarlayout.h"
#include "phoneflipcontrol.h"
#ifndef SAILFISH
#include <MDeclarativeCache>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MPannableViewport>
#include <MPositionIndicator>
#else
#include <MDeclarativeCache>
#include <QGuiApplication>
#include <QQuickView>
#include <QtQuick>
#include <QQmlError>
#endif
#include "models.h"
#include "bookmarksmodel.h"
#include "bookmarkitem.h"
#include "quranviewmodel2.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

#define FONT_FILE                  ":/SimplifiedNaskh.ttf"

Q_DECL_EXPORT int main(int argc, char *argv[]) {
#ifdef SAILFISH
  QGuiApplication *app = MDeclarativeCache::qApplication(argc, argv);
  app->setApplicationName("quran");
  app->setApplicationDisplayName(QObject::tr("Holy Quran"));

  QQuickView *view = MDeclarativeCache::qQuickView();
  view->setTitle(app->applicationDisplayName());
  view->setResizeMode(QQuickView::SizeRootObjectToView);
  QQmlEngine *engine = view->engine();
  QQmlContext *rootContext = engine->rootContext();
#else
  MApplication *app = new MApplication(argc, argv);
#endif

  QFontDatabase::addApplicationFont(FONT_FILE);

  avcodec_register_all();
  av_register_all();

  bool dev = false;
  for (int x = 0; x < argc; x++) {
    if (QLatin1String("-quran-dev") == QLatin1String(argv[x])) {
      dev = true;
      break;
    }
  }

  qmlRegisterType<DataProvider>("Quran", 1, 0, "DataProvider");
  qmlRegisterType<Settings>("Quran", 1, 0, "Settings");
  qmlRegisterType<Downloader>("Quran", 1, 0, "Downloader");
  qmlRegisterType<Bookmarks>("Quran", 1, 0, "Bookmarks");
  qmlRegisterType<NumberFormatter>("Quran", 1, 0, "NumberFormatter");
  qmlRegisterType<AboutData>("Quran", 1, 0, "AboutData");
  qmlRegisterType<Translations>("Quran", 1, 0, "Translations");
  qmlRegisterType<Recitations>("Quran", 1, 0, "Recitations");
  qmlRegisterType<FSMonitor>("Quran", 1, 0, "FSMonitor");
  qmlRegisterType<Search>("Quran", 1, 0, "Search");
  qmlRegisterType<Colors>("Quran", 1, 0, "Colors");
  qmlRegisterType<Translation>("Quran", 1, 0, "Translation");
  qmlRegisterType<QuranViewModel>("Quran", 1, 0, "QuranViewModel");
#ifndef SAILFISH
  qmlRegisterType<WindowController>("Quran", 1, 0, "WindowController");
#endif
#ifdef SAILFISH
  qmlRegisterType<ToolBarLayout>("Quran", 1, 0, "ToolBarLayout");
#endif
  qmlRegisterType<PhoneFlipControl>("Quran", 1, 0, "PhoneFlipControl");
  qmlRegisterType<InstalledTranslationsModel>("Quran", 1, 0, "InstalledTranslationsModel");
  qmlRegisterType<ActiveTranslationsModel>("Quran", 1, 0, "ActiveTranslationsModel");
  qmlRegisterType<TranslationCategoriesModel>("Quran", 1, 0, "TranslationCategoriesModel");
  qmlRegisterType<TranslationCollection>("Quran", 1, 0, "TranslationCollection");
  qmlRegisterType<BookmarksModel>("Quran", 1, 0, "BookmarksModel");
  qmlRegisterType<BookmarkItem>("Quran", 1, 0, "BookmarkItem");
  qmlRegisterType<QuranViewModel2>("Quran", 1, 0, "QuranViewModel2");

#ifndef SAILFISH
  MApplicationWindow *view = new MApplicationWindow;
  view->setRoundedCornersEnabled(false);
  MApplicationPage *page = new MApplicationPage();
  page->setPannable(false);
  page->appear();
  page->pannableViewport()->positionIndicator()->hide();
  QDeclarativeEngine *engine = new QDeclarativeEngine;

  QDeclarativeContext *rootContext = engine->rootContext();

  view->setWindowTitle(QObject::tr("Holy Quran"));
#endif

  QObject::connect(engine, SIGNAL(quit()), app, SLOT(quit()));

  ThemeImageProvider *theme = new ThemeImageProvider(DATA_DIR "/themes/");

  engine->addImageProvider("theme", theme);

  rootContext->setContextProperty("_theme", theme);

  QUrl sourceUrl = dev ? QUrl::fromLocalFile(QDir::currentPath() + "/main.qml")
    : QUrl("qrc:/qml/main.qml");

#ifdef SAILFISH
  view->setSource(QUrl("qrc:/qml/main.qml"));
  if (view->status() == QQuickView::Error) {
    qCritical() << "Errors loading QML:";
    QList<QQmlError> errors = view->errors();

    foreach (const QQmlError& error, errors) {
      qCritical() << error.toString();
    }

    delete view;
    delete app;

    return 1;
  }

  view->show();

#else
  QDeclarativeItem *root = 0;

  QDeclarativeComponent component(engine, sourceUrl);
  if (component.isError()) {
    QList<QDeclarativeError> errors = component.errors();

    foreach (const QDeclarativeError& error, errors) {
      qWarning() << error.toString();
    }

    return 1;
  }

  QGraphicsObject *content = qobject_cast<QGraphicsObject *>(component.create());
  root = qobject_cast<QDeclarativeItem *>(content);

  MWidget *centralWidget = new MWidget;
  content->setParentItem(centralWidget);
  centralWidget->setMinimumSize(864, 400);
  page->setCentralWidget(centralWidget);
#endif

  int ret = app->exec();

  delete view;
  delete app;

  return ret;
}
