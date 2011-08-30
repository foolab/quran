TEMPLATE = app

# // TODO: move this from here
TARGET = n900-quran

QT += network

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/

DEFINES += VERSION=\\\"0.3.1\\\" DATA_DIR=\\\"/usr/share/n900-quran/\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp legal.cpp logoprovider.cpp windowcontroller.cpp \
           translations.cpp translation.cpp downloader.cpp translation_p.cpp textprovider.cpp \
           index.cpp label.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h quranviewmodel.h \
           position.h legal.h logoprovider.h windowcontroller.h trans-meta.h \
           translations.h translation.h downloader.h translation_p.h textprovider.h \
           index.h label.h

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc
