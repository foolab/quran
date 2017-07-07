include(../../common.pri)

TARGET = search

SOURCES += main.cpp

DEPENDPATH += . ../../sqlite/
INCLUDEPATH += . ../../sqlite/

CONFIG += link_pkgconfig
PKGCONFIG += sqlite3

QMAKE_POST_LINK += ./search quran-simple-clean.xml ../search.db

QMAKE_CLEAN += ../search.db
