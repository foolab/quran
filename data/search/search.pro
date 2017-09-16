QT_CONFIG -= no-pkg-config

TARGET = search

SOURCES += main.cpp

# We depend on system sqlite
#DEPENDPATH += . ../../sqlite/
#INCLUDEPATH += . ../../sqlite/

CONFIG += link_pkgconfig
PKGCONFIG += sqlite3

POST_COMMAND = ./search quran-simple-clean.xml search.db && mv search.db ../

POST_OUT = ../search.db

include(../data.pri)
