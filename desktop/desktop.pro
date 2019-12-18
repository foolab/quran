VPATH += . ../data/ ../common/

TARGET = quran

QT += qml quick sensors

CONFIG += desktop alsa

# Has to be after android keyword in config
include(../common/common.pri)

DEFINES += DATA_DIR=\\\"/usr/share/quran/\\\"
DEFINES += DESKTOP=1

RESOURCES += ../android/android.qrc ../icons/icons.qrc ../qml/qml.qrc

SOURCES += fsmonitor.cpp \
           audiopolicy.cpp \
           application.cpp \
           mockandroidsupport.cpp \
           mediaservice.cpp \
           ../android/keyfilter.cpp

HEADERS += fsmonitor.h \
           audiopolicy.h \
           application.h \
           mockandroidsupport.h \
           mediaservice.h \
           ../android/keyfilter.h
