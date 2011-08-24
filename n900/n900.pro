include(../common/common.pri)

QT += maemo5 declarative
# CONFIG += debug
CONFIG += link_pkgconfig
PKGCONFIG += gio-2.0

SOURCES += main.cpp themeimageprovider.cpp fsmonitor.cpp

HEADERS += themeimageprovider.h fsmonitor.h
