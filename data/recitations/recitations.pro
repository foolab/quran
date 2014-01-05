TEMPLATE = app
TARGET = recitations
DEPENDPATH += .
INCLUDEPATH += .

QT = core script

SOURCES += main.cpp

DEPENDPATH += .

QMAKE_POST_LINK += ./recitations recitations.js > ../recite-meta.h

QMAKE_CLEAN += ../recite-meta.h
