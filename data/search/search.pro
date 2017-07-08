TARGET = search

SOURCES += main.cpp

DEPENDPATH += . ../../sqlite/
INCLUDEPATH += . ../../sqlite/

CONFIG += link_pkgconfig
PKGCONFIG += sqlite3

POST_COMMAND = ./search quran-simple-clean.xml search.db && mv search.db ../

POST_OUT = ../search.db

include(../../common.pri)
