include(../common/common.pri)

VPATH += . ../data/ ../common/

TARGET = harbour-quran

QT += qml quick

CONFIG += link_pkgconfig meegotouch sailfish

PKGCONFIG += qdeclarative5-boostable sqlite3 Qt5Sensors audioresource-qt

SOURCES += fsmonitor.cpp audiopolicy.cpp

HEADERS += fsmonitor.h audiopolicy.h

INCLUDEPATH += libav/

DEFINES += DATA_DIR=\\\"/usr/share/harbour-quran/\\\"
DEFINES += SAILFISH=1

RESOURCES += qml.qrc

LIBS += -Wl,-rpath=/usr/share/harbour-quran/lib/ \
        -Llibav/libavformat/ -lavformat \
        -Llibav/libavcodec/ -lavcodec \
        -Llibav/libavutil/ -lavutil \
        -Llibav/libavfilter/ -lavfilter \
        -Llibav/libavresample/ -lavresample \
        -Lsqlite/ -lsqlite3 \
        -Lquazip/quazip/ -lquazip
