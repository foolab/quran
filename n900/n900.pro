include(../common/common.pri)

QT += maemo5 declarative
# CONFIG += debug
CONFIG += link_pkgconfig
PKGCONFIG += gio-2.0 libplayback-1 libosso

SOURCES += main.cpp themeimageprovider.cpp fsmonitor.cpp audiopolicy.cpp

HEADERS += themeimageprovider.h fsmonitor.h audiopolicy.h

LIBS += ../sqlite/.libs/libsqlite3.a
