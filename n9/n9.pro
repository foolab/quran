include(../common/common.pri)

TARGET = n9-quran

QT += declarative

CONFIG += link_pkgconfig meegotouch debug

PKGCONFIG += qmsystem2 qdeclarative-boostable sqlite3

SOURCES += fsmonitor.cpp audiopolicy.cpp windowcontroller.cpp

HEADERS += fsmonitor.h audiopolicy.h windowcontroller.h

DEFINES += DATA_DIR=\\\"/opt/n9-quran/\\\"

RESOURCES += ../qml.qrc
