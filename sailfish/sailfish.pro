VPATH += . ../data/ ../common/

TARGET = harbour-quran

QT += qml quick

CONFIG += link_pkgconfig sailfish pulse

# Has to be after sailfish keyword in config
include(../common/common.pri)

PKGCONFIG += qdeclarative5-boostable Qt5Sensors libresourceqt5

DEFINES += DATA_DIR=\\\"/usr/share/harbour-quran/\\\"
DEFINES += SAILFISH=1

RESOURCES += sailfish.qrc ../icons/icons.qrc ../qml/qml.qrc
SOURCES += fsmonitor.cpp audiopolicy.cpp application.cpp mediaservice.cpp networkmanager.cpp
HEADERS += fsmonitor.h audiopolicy.h mediaservice.h

LIBS += -Wl,-rpath=/usr/share/harbour-quran/lib/
