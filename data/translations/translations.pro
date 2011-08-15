TEMPLATE = app
TARGET = translations
DEPENDPATH += .
INCLUDEPATH += .

QT = core

SOURCES += main.cpp

DEPENDPATH += .

QMAKE_POST_LINK += ./translations trans.ini > ../trans-meta.h

QMAKE_CLEAN += ../trans-meta.h
