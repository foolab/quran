TEMPLATE = app

CONFIG += c++11

QT += network
CONFIG += mobility link_pkgconfig
MOBILITY = sensors
!android:PKGCONFIG += libpulse fontconfig
android:LIBS += -lOpenSLES

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/ ../libav/ ../sqlite/ ../quazip/quazip/

DEFINES += __STDC_CONSTANT_MACROS VERSION=\\\"0.6.6\\\"

equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5

GENERATED = ../data/metadata.h ../data/trans-meta.h ../data/recite-meta.h

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           position.cpp aboutdata.cpp textsupplier.cpp \
           translations.cpp translation.cpp downloader.cpp textprovider.cpp \
           index.cpp recitations.cpp recitation.cpp mediaplaylist.cpp \
           main.cpp phoneflipcontrol.cpp mediaplayer.cpp iconprovider.cpp \
           media.cpp audiooutput.cpp mediadecoder.cpp bookmarksmodel.cpp \
           bookmarkitem.cpp mediaresampler.cpp searchmodel.cpp models.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h quranviewmodel.h \
           position.h aboutdata.h textsupplier.h \
           translations.h translation.h downloader.h textprovider.h \
           index.h recitations.h recitation.h mediaplaylist.h \
           phoneflipcontrol.h mediaplayer.h iconprovider.h \
           media.h audiooutput.h mediadecoder.h bookmarksmodel.h \
           bookmarkitem.h mediaresampler.h searchmodel.h models.h \
           $$GENERATED

android {
SOURCES += sles.cpp
HEADERS += sles.h
} else {
SOURCES += pulse.cpp
HEADERS += pulse.h
}

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc ../theme.qrc ../logo.qrc
QMAKE_RESOURCE_FLAGS += -no-compress

PRE_TARGETDEPS += gen

gen.depends = gen_text gen_search gen_recitations gen_translations

gen_text.commands = cd ../data/text/ && $$QMAKE_QMAKE && make gen
gen_search.commands = cd ../data/search/ && $$QMAKE_QMAKE && make gen
gen_recitations.commands = cd ../data/recitations/ && $$QMAKE_QMAKE && make gen
gen_translations.commands = cd ../data/translations/ && $$QMAKE_QMAKE && make gen

QMAKE_EXTRA_TARGETS += gen gen_text gen_search gen_recitations gen_translations
