TEMPLATE = app

# // TODO: move this from here
TARGET = n900-quran

QT += network

CONFIG += mobility

MOBILITY = multimedia

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/

DEFINES += VERSION=\\\"0.3.10\\\" DATA_DIR=\\\"/usr/share/n900-quran/\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp aboutdata.cpp logoprovider.cpp windowcontroller.cpp \
           translations.cpp translation.cpp downloader.cpp translation_p.cpp textprovider.cpp \
           index.cpp label.cpp search.cpp recitations.cpp recitation.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h quranviewmodel.h \
           position.h aboutdata.h logoprovider.h windowcontroller.h trans-meta.h \
           translations.h translation.h downloader.h translation_p.h textprovider.h \
           index.h label.h search.h recitations.h recitation.h

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc
