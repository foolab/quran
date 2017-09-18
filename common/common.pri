TEMPLATE = app

CONFIG += c++11

QT += network
CONFIG += mobility link_pkgconfig
MOBILITY = sensors
!android:PKGCONFIG += libpulse fontconfig
android:LIBS += -lOpenSLES

DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/ ../libav/

!desktop:INCLUDEPATH += ../sqlite/ ../quazip/quazip/

DEFINES += __STDC_CONSTANT_MACROS VERSION=\\\"0.6.6\\\"

GENERATED = ../data/text-meta.h

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp quranviewmodel.cpp \
           aboutdata.cpp textsupplier.cpp \
           translations.cpp translation.cpp downloader.cpp textprovider.cpp \
           index.cpp recitations.cpp recitation.cpp mediaplaylist.cpp \
           main.cpp phoneflipcontrol.cpp mediaplayer.cpp iconprovider.cpp \
           media.cpp audiooutput.cpp mediadecoder.cpp bookmarksmodel.cpp \
           bookmarkitem.cpp mediaresampler.cpp searchmodel.cpp models.cpp \
           coreapplication.cpp metadata.cpp chapterinfo.cpp partinfo.cpp pageinfo.cpp \
           pageposition.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h quranviewmodel.h \
           aboutdata.h textsupplier.h \
           translations.h translation.h downloader.h textprovider.h \
           index.h recitations.h recitation.h mediaplaylist.h \
           phoneflipcontrol.h mediaplayer.h iconprovider.h \
           media.h audiooutput.h mediadecoder.h bookmarksmodel.h \
           bookmarkitem.h mediaresampler.h searchmodel.h models.h \
           coreapplication.h metadata.h chapterinfo.h partinfo.h pageinfo.h \
           pageposition.h \
           $$GENERATED

sles {
  SOURCES += sles.cpp
  HEADERS += sles.h
  DEFINES += SLES
}

pulse {
  SOURCES += pulse.cpp
  HEADERS += pulse.h
  DEFINES += PULSE
}

alsa {
  SOURCES += alsa.cpp
  HEADERS += alsa.h
  DEFINES += ALSA
}

target.path = /usr/bin/

INSTALLS += target

RESOURCES += data.qrc ../theme.qrc ../logo.qrc
QMAKE_RESOURCE_FLAGS += -no-compress

PRE_TARGETDEPS += gen

gen.depends = gen_text gen_search gen_recitations gen_translations

defineReplace(build_dep) {
  android:QMAKE_ARGS=QMAKE_PKG_CONFIG=pkg-config
  return(cd ../data/$$1/ && $$QMAKE_QMAKE $$QMAKE_ARGS && make gen)
}

gen_text.commands = $$build_dep(text)
gen_search.commands = $$build_dep(search)
gen_recitations.commands = $$build_dep(recitations)
gen_translations.commands = $$build_dep(translations)

QMAKE_EXTRA_TARGETS += gen gen_text gen_search gen_recitations gen_translations
