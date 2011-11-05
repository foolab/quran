include(../common/common.pri)

TARGET = n900-quran

QT += maemo5 declarative

# CONFIG += debug
CONFIG += link_pkgconfig

PKGCONFIG += gio-2.0 libplayback-1 libosso

SOURCES += fsmonitor.cpp audiopolicy.cpp

HEADERS += fsmonitor.h audiopolicy.h

LIBS += ../sqlite/.libs/libsqlite3.a
