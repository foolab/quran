include(../common/common.pri)

TARGET = n9-quran

QT += declarative

CONFIG += link_pkgconfig meegotouch debug

PKGCONFIG += qmsystem2 qdeclarative-boostable sqlite3 libresourceqt1

SOURCES += fsmonitor.cpp windowcontroller.cpp

HEADERS += fsmonitor.h windowcontroller.h

DEFINES += DATA_DIR=\\\"/opt/n9-quran/\\\"

RESOURCES += qml.qrc

LIBS += -Wl,--whole-archive \
        -Llibav/libavformat/ -lavformat \
        -Llibav/libavcodec/ -lavcodec \
        -Llibav/libavutil/ -lavutil \
        -Llibav/libavfilter/ -lavfilter \
        -Llibav/libavresample/ -lavresample \
        -Wl,--no-whole-archive

INCLUDEPATH += . libav/
