VPATH += . ../data/ ../common/

TARGET = Quran

QT += qml quick sensors androidextras

CONFIG += android sles
SQLITE_EXTRA_COMPILE_FLAGS += --host=arm-linux-androideabi \
                              --with-sysroot=$$(SYSROOT)

FFMPEG_EXTRA_COMPILE_FLAGS += --enable-cross-compile \
                              --target-os=android \
                              --disable-runtime-cpudetect \
                              --arch=arm \
                              --cpu=cortex-a8 \
                              --cc=$$(CC) \
                              --ld=$$(LD) \
                              --nm=$$(NM) \
                              --strip=$$(STRIP) \
                              --extra-cflags=\"$$(ANDROID_CFLAGS)\" \
                              --extra-ldflags=\"$$(ANDROID_LDFLAGS)\"

# Has to be after android keyword in config
include(../common/common.pri)

DEFINES += DATA_DIR=\\\"assets:/\\\"
DEFINES += ANDROID=1

RESOURCES += android.qrc ../icons/icons.qrc ../qml/qml.qrc
SOURCES += fsmonitor.cpp \
           audiopolicy.cpp \
           application.cpp \
           androidsupport.cpp \
           keyfilter.cpp \
           sqlite-ndk/sources/sqlite3ndk.cpp
HEADERS += fsmonitor.h \
           audiopolicy.h \
           application.h \
           androidsupport.h \
           keyfilter.h \
           sqlite-ndk/sources/sqlite3ndk.h

LIBS += -landroid

ANDROID_PACKAGE_SOURCE_DIR = android/apk
