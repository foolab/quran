TARGET = metadata

SOURCES += main.cpp index.cpp
HEADERS += index.h

POST_OUT = ../metadata.h
POST_COMMAND = ./metadata quran-data.xml OTTOMAN:quran.xml:../quran.idx SIMPLE:quran-simple-enhanced.xml:../quran-simple-enhanced.idx > metadata.h && mv metadata.h ../

include(../../common.pri)

