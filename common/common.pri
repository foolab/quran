TEMPLATE = app

include(../quazip/quazip/quazip.pri)

QT += network

CONFIG += mobility link_pkgconfig

PKGCONFIG += gstreamer-base-0.10

MOBILITY = multimedia sensors

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/

DEFINES += VERSION=\\\"0.5.3\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp aboutdata.cpp logoprovider.cpp windowcontroller.cpp \
           translations.cpp translation.cpp downloader.cpp translation_p.cpp textprovider.cpp \
           index.cpp label.cpp search.cpp recitations.cpp recitation.cpp mediaplaylist.cpp \
           main.cpp themeimageprovider.cpp gstzipsrc.cpp phoneflipcontrol.cpp colors.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h quranviewmodel.h \
           position.h aboutdata.h logoprovider.h windowcontroller.h trans-meta.h \
           translations.h translation.h downloader.h translation_p.h textprovider.h \
           index.h label.h search.h recitations.h recitation.h mediaplaylist.h \
           themeimageprovider.h gstzipsrc.h phoneflipcontrol.h colors.h

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc
