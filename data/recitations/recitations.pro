TARGET = recitations

SOURCES += main.cpp

POST_COMMAND = ./recitations recite.ini > recite-meta.h && mv recite-meta.h ../

POST_OUT = ../recite-meta.h

include(../data.pri)
