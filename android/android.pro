VPATH += . ../data/ ../common/

TARGET = Quran

QT += qml quick sensors androidextras

CONFIG += android sles

# Has to be after android keyword in config
include(../common/common.pri)

INCLUDEPATH += libav/

DEFINES += DATA_DIR=\\\"assets:/\\\"
DEFINES += ANDROID=1

RESOURCES += android.qrc ../icons/icons.qrc ../qml/qml.qrc
SOURCES += fsmonitor.cpp \
           audiopolicy.cpp \
           application.cpp \
           androidsupport.cpp \
           sqlite-ndk/sources/sqlite3ndk.cpp
HEADERS += fsmonitor.h \
           audiopolicy.h \
           application.h \
           androidsupport.h \
           sqlite-ndk/sources/sqlite3ndk.h

LIBS += -Llibav/libavformat/ -lavformat \
        -Llibav/libavcodec/ -lavcodec \
        -Llibav/libavutil/ -lavutil \
        -Llibav/libavfilter/ -lavfilter \
        -Llibav/libavresample/ -lavresample \
        -Lsqlite/.libs/ -lsqlite3 \
        -Lquazip/quazip/ -lquazip \
        -landroid

ANDROID_EXTRA_LIBS = android/libav/libavutil/libavutil.so \
                     android/libav/libavcodec/libavcodec.so \
                     android/libav/libavformat/libavformat.so \
                     android/libav/libavfilter/libavfilter.so \
                     android/libav/libavresample/libavresample.so \
                     android/sqlite/.libs/libsqlite3.so \
                     android/quazip/quazip/libquazip.so

ANDROID_PACKAGE_SOURCE_DIR = android/apk
