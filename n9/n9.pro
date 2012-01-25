include(../common/common.pri)

TARGET = n9-quran

QT += declarative

CONFIG += link_pkgconfig meegotouch debug

PKGCONFIG += qmsystem2 qdeclarative-boostable libresourceqt1 sqlite3

SOURCES += fsmonitor.cpp audiopolicy.cpp

HEADERS += fsmonitor.h audiopolicy.h

DEFINES += DATA_DIR=\\\"/opt/n9-quran/\\\"
