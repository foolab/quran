TEMPLATE = app
TARGET = metadata
DEPENDPATH += .
INCLUDEPATH += .

QT = core

SOURCES += main.cpp index.cpp
HEADERS += index.h

VPATH += ../../common/
DEPENDPATH += . ../../common/
INCLUDEPATH += . ../../common/

QMAKE_POST_LINK += ./metadata quran-data.xml OTTOMAN:quran.xml:../quran.idx SIMPLE:quran-simple-enhanced.xml:../quran-simple-enhanced.idx > ../metadata.h

QMAKE_CLEAN += ../metadata.h
