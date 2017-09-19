VPATH += . ../data/ ../common/

TARGET = quran

QT += qml quick sensors

CONFIG += link_pkgconfig desktop alsa
PKGCONFIG += sqlite3 alsa

# Has to be after android keyword in config
include(../common/common.pri)

DEFINES += DATA_DIR=\\\"/usr/share/quran/\\\"
DEFINES += DESKTOP=1

RESOURCES += ../android/android.qrc ../icons/icons.qrc ../qml/qml.qrc
SOURCES += fsmonitor.cpp audiopolicy.cpp application.cpp mockandroidsupport.cpp
HEADERS += fsmonitor.h audiopolicy.h application.h mockandroidsupport.h
INCLUDEPATH += /usr/include/quazip5/
LIBS += -lquazip5 -lavformat -lavcodec -lavutil -lavresample
