TEMPLATE = app
TARGET = metadata
DEPENDPATH += .
INCLUDEPATH += .

QT = core

SOURCES += main.cpp

DEPENDPATH += .

QMAKE_POST_LINK += ./metadata quran-data.xml OTTOMAN:quran-uthmani.xml SIMPLE:quran-simple-enhanced.xml > ../metadata.h

QMAKE_CLEAN += ../metadata.h
