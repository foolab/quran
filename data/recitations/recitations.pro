include(../../common.pri)

TARGET = recitations

SOURCES += main.cpp

QMAKE_POST_LINK += ./recitations recite.ini > ../recite-meta.h

QMAKE_CLEAN += ../recite-meta.h
