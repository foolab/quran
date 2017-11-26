VPATH += . ../data/ ../common/

TARGET = Quran

QT += qml quick sensors androidextras

CONFIG += android sles

SQLITE_EXTRA_COMPILE_FLAGS += --host=arm-linux-androideabi \
                              --with-sysroot=$$(SYSROOT)

LIBAV_EXTRA_COMPILE_FLAGS += --enable-cross-compile \
                            --sysroot=$$(SYSROOT) \
                            --target-os=android \
                            --arch=arm \
                            --cross-prefix=$$(TOOLCHAIN)/arm-linux-androideabi- \
                            --extra-cflags=\"-march=armv7-a -mfloat-abi=softfp\" \
                            --extra-ldflags=\"-Wl,--fix-cortex-a8\"

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
