TARGET = translations

SOURCES += main.cpp

POST_COMMAND = ./translations trans.ini > trans-meta.h && mv trans-meta.h ../

POST_OUT = ../trans-meta.h

include(../../common.pri)
