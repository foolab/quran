include(../../common.pri)

TARGET = metadata

SOURCES += main.cpp index.cpp
HEADERS += index.h

QMAKE_POST_LINK += ./metadata quran-data.xml OTTOMAN:quran.xml:../quran.idx SIMPLE:quran-simple-enhanced.xml:../quran-simple-enhanced.idx > ../metadata.h

QMAKE_CLEAN += ../metadata.h
