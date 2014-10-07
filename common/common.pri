TEMPLATE = app

QT += network

CONFIG += mobility link_pkgconfig
MOBILITY = sensors
PKGCONFIG += libpulse fontconfig

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/ ../libav/ ../sqlite/ ../quazip/quazip/

DEFINES += __STDC_CONSTANT_MACROS VERSION=\\\"0.6.3\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp aboutdata.cpp \
           translations.cpp translation.cpp downloader.cpp translation_p.cpp textprovider.cpp \
           index.cpp search.cpp recitations.cpp recitation.cpp mediaplaylist.cpp \
           main.cpp themeimageprovider.cpp phoneflipcontrol.cpp colors.cpp mediaplayer.cpp \
           media.cpp audiooutput.cpp mediadecoder.cpp models.cpp bookmarksmodel.cpp \
           bookmarkitem.cpp pulse.cpp mediaresampler.cpp searchmodel.cpp audiopolicy.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h quranviewmodel.h \
           position.h aboutdata.h trans-meta.h recite-meta.h \
           translations.h translation.h downloader.h translation_p.h textprovider.h \
           index.h search.h recitations.h recitation.h mediaplaylist.h \
           themeimageprovider.h phoneflipcontrol.h colors.h mediaplayer.h \
           media.h audiooutput.h mediadecoder.h models.h bookmarksmodel.h \
           bookmarkitem.h pulse.h mediaresampler.h searchmodel.h audiopolicy.h

sailfish: {
SOURCES += iconprovider.cpp
HEADERS += iconprovider.h
}

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc ../theme.qrc ../logo.qrc
QMAKE_RESOURCE_FLAGS += -no-compress
