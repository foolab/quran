VPATH += . ../data/ ../common/

TARGET = harbour-quran

QT += qml quick

CONFIG += link_pkgconfig sailfish

# Has to be after sailfish keyword in config
include(../common/common.pri)

PKGCONFIG += qdeclarative5-boostable Qt5Sensors libresourceqt5

INCLUDEPATH += libav/

DEFINES += DATA_DIR=\\\"/usr/share/harbour-quran/\\\"
DEFINES += SAILFISH=1

RESOURCES += sailfish.qrc ../icons/icons.qrc ../qml/qml.qrc
SOURCES += fsmonitor.cpp audiopolicy.cpp
HEADERS += fsmonitor.h audiopolicy.h

LIBS += -Wl,-rpath=/usr/share/harbour-quran/lib/ \
        -Llibav/libavformat/ -lavformat \
        -Llibav/libavcodec/ -lavcodec \
        -Llibav/libavutil/ -lavutil \
        -Llibav/libavfilter/ -lavfilter \
        -Llibav/libavresample/ -lavresample \
        -Lsqlite/ -lsqlite3 \
        -Lquazip/quazip/ -lquazip
