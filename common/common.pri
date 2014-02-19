TEMPLATE = app

include(../quazip/quazip/quazip.pri)
QT += network

CONFIG += mobility link_pkgconfig
MOBILITY = sensors
PKGCONFIG += libpulse

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/ ../libav/

DEFINES += __STDC_CONSTANT_MACROS VERSION=\\\"0.5.3\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp aboutdata.cpp toolbarlayout.cpp \
           translations.cpp translation.cpp downloader.cpp translation_p.cpp textprovider.cpp \
           index.cpp search.cpp recitations.cpp recitation.cpp mediaplaylist.cpp \
           main.cpp themeimageprovider.cpp phoneflipcontrol.cpp colors.cpp mediaplayer.cpp \
           media.cpp audiooutput.cpp mediadecoder.cpp models.cpp bookmarksmodel.cpp \
           bookmarkitem.cpp quranviewmodel2.cpp pulse.cpp mediaresampler.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h quranviewmodel.h \
           position.h aboutdata.h trans-meta.h recite-meta.h toolbarlayout.h \
           translations.h translation.h downloader.h translation_p.h textprovider.h \
           index.h search.h recitations.h recitation.h mediaplaylist.h \
           themeimageprovider.h phoneflipcontrol.h colors.h mediaplayer.h \
           media.h audiooutput.h mediadecoder.h models.h bookmarksmodel.h \
           bookmarkitem.h quranviewmodel2.h pulse.h mediaresampler.h

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc ../theme.qrc ../logo.qrc
QMAKE_RESOURCE_FLAGS += -no-compress
