TEMPLATE = app
TARGET = search
DEPENDPATH += .
INCLUDEPATH += .

QT = core

SOURCES += main.cpp
HEADERS += sqlite3.h

DEPENDPATH += . ../../common/ ../../sqlite/
INCLUDEPATH += . ../../common/ ../../sqlite/

LIBS += ../../sqlite/.libs/libsqlite3.a

QMAKE_POST_LINK += ./search quran-simple-clean.xml ../search.db

QMAKE_CLEAN += ../search.db
