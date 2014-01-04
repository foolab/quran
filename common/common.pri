TEMPLATE = app

!sailfish {
include(../quazip/quazip/quazip.pri)
PKGCONFIG += gstreamer-base-0.10
SOURCES += gstzipsrc.cpp
HEADERS += gstzipsrc.h
}

QT += network

CONFIG += mobility link_pkgconfig

MOBILITY = multimedia sensors

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/

DEFINES += VERSION=\\\"0.5.3\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp aboutdata.cpp toolbarlayout.cpp \
           translations.cpp translation.cpp downloader.cpp translation_p.cpp textprovider.cpp \
           index.cpp search.cpp recitations.cpp recitation.cpp mediaplaylist.cpp \
           main.cpp themeimageprovider.cpp phoneflipcontrol.cpp colors.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h quranviewmodel.h \
           position.h aboutdata.h trans-meta.h toolbarlayout.h \
           translations.h translation.h downloader.h translation_p.h textprovider.h \
           index.h search.h recitations.h recitation.h mediaplaylist.h \
           themeimageprovider.h phoneflipcontrol.h colors.h

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc ../theme.qrc ../logo.qrc
QMAKE_RESOURCE_FLAGS += -no-compress
