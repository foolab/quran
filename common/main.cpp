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

#include "settings.h"
#include "dataprovider.h"
#include "bookmarks.h"
#include "numberformatter.h"
#include "aboutdata.h"
#include <QDir>
#include "translations.h"
#include "translation.h"
#include "downloader.h"
#include "fsmonitor.h"
#include "search.h"
#include "recitations.h"
#include "phoneflipcontrol.h"
#ifndef SAILFISH
#include <MDeclarativeCache>
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QtDeclarative>
#else
#include <MDeclarativeCache>
#include <QGuiApplication>
#include <QQuickView>
#include <QtQuick>
#include <QQmlError>
#endif
#include <QFontDatabase>
#include "models.h"
#include "bookmarksmodel.h"
#include "bookmarkitem.h"
#include "quranviewmodel.h"
#include "searchmodel.h"
#include "iconprovider.h"
#include "textsupplier.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};
#include <fontconfig/fontconfig.h>

#define FONTS_DIR         DATA_DIR"/fonts/"
#define FONTS_CONF        FONTS_DIR"/fonts.conf"

Q_DECL_EXPORT int main(int argc, char *argv[]) {
#ifdef SAILFISH
  QGuiApplication *app = MDeclarativeCache::qApplication(argc, argv);
  app->setApplicationName("harbour-quran");
  app->setApplicationDisplayName(QObject::tr("Holy Quran"));

  QQuickView *view = MDeclarativeCache::qQuickView();
  view->setTitle(app->applicationDisplayName());
  view->setResizeMode(QQuickView::SizeRootObjectToView);
  QQmlEngine *engine = view->engine();
#else
  QApplication *app = MDeclarativeCache::qApplication(argc, argv);
  QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();
  view->setWindowTitle(QObject::tr("Holy Quran"));
  view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
  QDeclarativeEngine *engine = view->engine();
#endif

  QFontDatabase::addApplicationFont(FONTS_DIR"/amiri-regular.ttf");
  QFontDatabase::addApplicationFont(FONTS_DIR"/SimplifiedNaskh.ttf");

  FcConfig *conf = FcConfigGetCurrent();
  FcConfigParseAndLoad(conf,  reinterpret_cast<const FcChar8 *>(FONTS_CONF), 1);
  FcConfigSetCurrent(conf);

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
  qmlRegisterType<SearchModel>("Quran", 1, 0, "SearchModel");
  qmlRegisterType<Translation>("Quran", 1, 0, "Translation");
  qmlRegisterType<PhoneFlipControl>("Quran", 1, 0, "PhoneFlipControl");
  qmlRegisterType<InstalledTranslationsModel>("Quran", 1, 0, "InstalledTranslationsModel");
  qmlRegisterType<ActiveTranslationsModel>("Quran", 1, 0, "ActiveTranslationsModel");
  qmlRegisterType<TranslationCategoriesModel>("Quran", 1, 0, "TranslationCategoriesModel");
  qmlRegisterType<TranslationCollection>("Quran", 1, 0, "TranslationCollection");
  qmlRegisterType<RecitationModel>("Quran", 1, 0, "RecitationModel");
  qmlRegisterType<InstallableRecitationsModel>("Quran", 1, 0, "InstallableRecitationsModel");
  qmlRegisterType<BookmarksModel>("Quran", 1, 0, "BookmarksModel");
  qmlRegisterType<BookmarkItem>("Quran", 1, 0, "BookmarkItem");
  qmlRegisterType<QuranViewModel>("Quran", 1, 0, "QuranViewModel");
  qmlRegisterType<TextSupplier>("Quran", 1, 0, "TextSupplier");

  QObject::connect(engine, SIGNAL(quit()), app, SLOT(quit()));

  engine->addImageProvider(QLatin1String("icon"), new IconProvider);

  QUrl sourceUrl = dev ? QUrl::fromLocalFile(QDir::currentPath() + "/main.qml")
    : QUrl("qrc:/qml/main.qml");

  view->setSource(QUrl("qrc:/qml/main.qml"));

#ifdef SAILFISH
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

#else
  if (view->status() == QDeclarativeView::Error) {
    qCritical() << "Errors loading QML:";
    QList<QDeclarativeError> errors = view->errors();

    foreach (const QDeclarativeError& error, errors) {
      qCritical() << error.toString();
    }

    delete view;
    delete app;
    return 1;
  }
#endif
  view->showFullScreen();

  int ret = app->exec();

  delete view;
  delete app;

  return ret;
}
