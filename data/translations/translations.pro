include(../../common.pri)

TARGET = translations

SOURCES += main.cpp

QMAKE_POST_LINK += ./translations trans.ini > ../trans-meta.h

QMAKE_CLEAN += ../trans-meta.h
