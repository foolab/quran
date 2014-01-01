VPATH += . ../data/ ../common/

TARGET = harbour-quran

QT += qml quick

CONFIG += link_pkgconfig meegotouch debug sailfish

PKGCONFIG += qdeclarative5-boostable sqlite3 Qt5Multimedia Qt5Sensors

SOURCES += fsmonitor.cpp audiopolicy.cpp

HEADERS += fsmonitor.h audiopolicy.h

DEFINES += DATA_DIR=\\\"/opt/n9-quran/\\\"
DEFINES += SAILFISH=1
LIBS += -lz

include(../common/common.pri)

RESOURCES += qml.qrc
