VPATH += . ../data/ ../common/

TARGET = quran

QT += qml quick sensors

CONFIG += link_pkgconfig desktop
PKGCONFIG += sqlite3

# Has to be after android keyword in config
include(../common/common.pri)

DEFINES += DATA_DIR=\\\"/usr/share/quran/\\\"
DEFINES += DESKTOP=1

RESOURCES += ../android/android.qrc ../icons/icons.qrc ../qml/qml.qrc
SOURCES += fsmonitor.cpp audiopolicy.cpp application.cpp
HEADERS += fsmonitor.h audiopolicy.h application.h

LIBS += -lquazip-qt5 -lavformat -lavcodec -lavutil -lavresample
