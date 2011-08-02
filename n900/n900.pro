TEMPLATE = app
TARGET = quran
DEPENDPATH += . ../src/ ../data/
INCLUDEPATH += . ../src/ ../data/
DEFINES += VERSION=\\\"0.1\\\" DATA_DIR=\\\"/usr/share/quran/\\\"

QT += maemo5

CONFIG += qdeclarative-boostable

SOURCES += main.cpp mainwindow.cpp settings.cpp dataprovider.cpp bookmarks.cpp numberformatter.cpp

#settings.cpp dataprovider.cpp quranview.cpp bookmarks.cpp numberformatter.cpp \
#           pagepositioncontroller.cpp imageprovider.cpp

HEADERS += mainwindow.h settings.h dataprovider.h bookmarks.h numberformatter.h metadata.h
#settings.h dataprovider.h metadata.h quranview.h bookmarks.h numberformatter.h \
           #pagepositioncontroller.h imageprovider.h

target.path = /usr/bin/

INSTALLS += target
