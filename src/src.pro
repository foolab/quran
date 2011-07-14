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

TEMPLATE = app
TARGET = quran
DEPENDPATH += .
INCLUDEPATH += .

QT += declarative

SOURCES += main.cpp settings.cpp dataprovider.cpp quranview.cpp bookmarks.cpp numberformatter.cpp \
           pagepositioncontroller.cpp imageprovider.cpp

HEADERS += settings.h dataprovider.h metadata.h quranview.h bookmarks.h numberformatter.h \
           pagepositioncontroller.h imageprovider.h

DEFINES += VERSION=\\\"0.1\\\" DATA_DIR=\\\"/usr/share/quran/\\\"

DEPENDPATH += ../data/
INCLUDEPATH += ../data/
