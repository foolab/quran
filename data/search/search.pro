TEMPLATE = app
TARGET = search
DEPENDPATH += .
INCLUDEPATH += .

QT = core

SOURCES += main.cpp

DEPENDPATH += . ../../common/ ../../sqlite/
INCLUDEPATH += . ../../common/ ../../sqlite/

CONFIG += link_pkgconfig
PKGCONFIG += sqlite3

QMAKE_POST_LINK += ./search quran-simple-clean.xml ../search.db

QMAKE_CLEAN += ../search.db
