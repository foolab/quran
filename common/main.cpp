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
#include "translations.h"
#include "downloader.h"
#include "fsmonitor.h"
#include "search.h"
#include "recitations.h"
#include "phoneflipcontrol.h"
#include "application.h"
#include <QFontDatabase>
#include "bookmarksmodel.h"
#include "bookmarkitem.h"
#include "quranviewmodel.h"
#include "searchmodel.h"
#include "iconprovider.h"
#include "textsupplier.h"
#include "translation.h"
#include "recitation.h"
#include "mediaplayer.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};
#ifndef ANDROID
#include <fontconfig/fontconfig.h>
#endif

#define FONTS_DIR         DATA_DIR"/fonts/"
#define FONTS_CONF        FONTS_DIR"/fonts.conf"

Q_DECL_EXPORT int main(int argc, char *argv[]) {
  Application app(argc, argv);

  QFontDatabase::addApplicationFont(FONTS_DIR"/amiri-regular.ttf");
  QFontDatabase::addApplicationFont(FONTS_DIR"/SimplifiedNaskh.ttf");

#ifndef ANDROID
  FcConfig *conf = FcConfigGetCurrent();
  FcConfigParseAndLoad(conf,  reinterpret_cast<const FcChar8 *>(FONTS_CONF), 1);
  FcConfigSetCurrent(conf);
#endif

  avcodec_register_all();
  av_register_all();

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
  qmlRegisterType<PhoneFlipControl>("Quran", 1, 0, "PhoneFlipControl");
  qmlRegisterType<BookmarksModel>("Quran", 1, 0, "BookmarksModel");
  qmlRegisterType<BookmarkItem>("Quran", 1, 0, "BookmarkItem");
  qmlRegisterType<QuranViewModel>("Quran", 1, 0, "QuranViewModel");
  qmlRegisterType<TextSupplier>("Quran", 1, 0, "TextSupplier");
  qmlRegisterType<MediaPlayer>("Quran", 1, 0, "MediaPlayer");
  qmlRegisterUncreatableType<Translation>("Quran", 1, 0, "Translation",
					  "Use Translations to obtain a Translation");
  qmlRegisterUncreatableType<Recitation>("Quran", 1, 0, "Recitation",
					  "Use Recitations to obtain a Recitation");

  app.registerQmlTypes();

  app.addImageProvider(QLatin1String("icon"), new IconProvider);

  if (!app.load(QUrl("qrc:/qml/main.qml"))) {
    return 1;
  }

  return app.exec();
}
