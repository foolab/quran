TARGET = metadata

SOURCES += main.cpp index.cpp
HEADERS += index.h

POST_OUT = ../text-meta.h
POST_COMMAND = ./metadata quran-data.xml OTTOMAN:quran.xml:../quran.idx SIMPLE:quran-simple-enhanced.xml:../quran-simple-enhanced.idx > text-meta.h && mv text-meta.h ../

include(../data.pri)
