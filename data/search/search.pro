TARGET = search

SOURCES += main.cpp \
           ../../common/normalize.cpp

INCLUDEPATH += . ../../common/

POST_COMMAND = ./search quran-simple-clean.xml search.db && mv search.db ../

POST_OUT = ../search.db

include(../data.pri)

SQLITE_SRC_PATH=../../sqlite
include(../../common/sqlite.pri)
