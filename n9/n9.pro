VPATH += . ../data/ ../common/

TARGET = n9-quran

QT += declarative

CONFIG += link_pkgconfig debug harmattan

# Has to be after harmattan keyword in config
include(../common/common.pri)

PKGCONFIG += qmsystem2 qdeclarative-boostable sqlite3 libresourceqt1

INCLUDEPATH += libav/

DEFINES += DATA_DIR=\\\"/opt/n9-quran/\\\"
DEFINES += HARMATTAN=1

RESOURCES += n9.qrc ../icons/icons.qrc ../qml/qml.qrc

LIBS += -Wl,-rpath=/opt/n9-quran/lib/ \
        -Llibav/libavformat/ -lavformat \
        -Llibav/libavcodec/ -lavcodec \
        -Llibav/libavutil/ -lavutil \
        -Llibav/libavfilter/ -lavfilter \
        -Llibav/libavresample/ -lavresample \
        -Lsqlite/ -lsqlite3 \
        -Lquazip/quazip/ -lquazip

SOURCES += fsmonitor.cpp
HEADERS += fsmonitor.h
