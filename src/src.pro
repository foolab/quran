TEMPLATE = app
TARGET = quran
DEPENDPATH += .
INCLUDEPATH += .

QT += declarative

SOURCES += main.cpp settings.cpp dataprovider.cpp quranview.cpp bookmarks.cpp numberformatter.cpp \
           pagepositioncontroller.cpp imageprovider.cpp

HEADERS += settings.h dataprovider.h metadata.h quranview.h bookmarks.h numberformatter.h \
           pagepositioncontroller.h imageprovider.h

DEFINES += VERSION=\\\"0.1\\\" DATA_DIR=\\\"/usr/share/quran/\\\"

DEPENDPATH += ../data/
INCLUDEPATH += ../data/
