TEMPLATE = app
TARGET = metadata
DEPENDPATH += .
INCLUDEPATH += .

QT = core

SOURCES += main.cpp

DEPENDPATH += .

gen.target = meta
gen.commands = ./metadata quran-data.xml OTTOMAN:quran-uthmani.xml SIMPLE:quran-simple-enhanced.xml > metadata.h
gen.depends =

QMAKE_EXTRA_TARGETS += gen
POST_TARGETDEPS += meta
