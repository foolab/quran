TEMPLATE = app

QT += network

CONFIG += mobility link_pkgconfig
MOBILITY = sensors
PKGCONFIG += libpulse fontconfig

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/ ../libav/ ../sqlite/ ../quazip/quazip/

DEFINES += __STDC_CONSTANT_MACROS VERSION=\\\"0.6.5\\\"

equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp aboutdata.cpp textsupplier.cpp \
           translations.cpp translation.cpp downloader.cpp translation_p.cpp textprovider.cpp \
           index.cpp search.cpp recitations.cpp recitation.cpp mediaplaylist.cpp \
           main.cpp phoneflipcontrol.cpp mediaplayer.cpp iconprovider.cpp \
           media.cpp audiooutput.cpp mediadecoder.cpp models.cpp bookmarksmodel.cpp \
           bookmarkitem.cpp pulse.cpp mediaresampler.cpp searchmodel.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h quranviewmodel.h \
           position.h aboutdata.h textsupplier.h trans-meta.h recite-meta.h \
           translations.h translation.h downloader.h translation_p.h textprovider.h \
           index.h search.h recitations.h recitation.h mediaplaylist.h \
           phoneflipcontrol.h mediaplayer.h iconprovider.h \
           media.h audiooutput.h mediadecoder.h models.h bookmarksmodel.h \
           bookmarkitem.h pulse.h mediaresampler.h searchmodel.h

harmattan {
SOURCES += qmlfileengine.cpp
HEADERS += qmlfileengine.h
}

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc ../theme.qrc ../logo.qrc
QMAKE_RESOURCE_FLAGS += -no-compress
