#!/bin/bash
set -e

QT_VERSION=5.3

export ANDROID_NDK_ROOT=/mnt/4/android/android-ndk-r10b
export ANDROID_SDK_ROOT=/mnt/4/android/android-sdk-linux/
export TOOLCHAIN=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86/bin/
export SYSROOT=$ANDROID_NDK_ROOT/platforms/android-9/arch-arm/
export CC=$TOOLCHAIN/arm-linux-androideabi-gcc
export CXX=$TOOLCHAIN/arm-linux-androideabi-g++
export NM=$TOOLCHAIN/arm-linux-androideabi-nm
export AR=$TOOLCHAIN/arm-linux-androideabi-ar
export LD=$TOOLCHAIN/arm-linux-androideabi-ld
export CFLAGS=--sysroot=$SYSROOT
export CPPFLAGS=--sysroot=$SYSROOT
export CXXFLAGS=--sysroot=$SYSROOT

rm -rf apk

mkdir -p android/libav
mkdir -p android/sqlite
mkdir -p android/quazip

pushd android/libav
../../libav/configure --disable-programs --disable-doc  --disable-avdevice --disable-swscale \
    --disable-network  --disable-everything --enable-sram --enable-bsf=mp3_header_decompress \
    --enable-demuxer=mp3 --enable-decoder=mp3 --enable-pic --disable-static --enable-shared \
    --enable-cross-compile --sysroot=$SYSROOT --target-os=android --arch=arm \
    --cross-prefix=$TOOLCHAIN/arm-linux-androideabi- \
    --extra-cflags="-march=armv7-a -mfloat-abi=softfp" \
    --extra-ldflags="-Wl,--fix-cortex-a8"

make
popd

pushd android/sqlite
FLAGS="-DSQLITE_ENABLE_FTS4=1 $CFLAGS" \
    ../../sqlite/configure --enable-readline=no \
    --enable-threadsafe=no --enable-shared=yes --enable-static=no --enable-dynamic-extensions=no \
    --host=arm-linux-androideabi --with-sysroot=$SYSROOT
sed -e 's/\$soname/libsqlite.so/' -i libtool
make
popd

pushd android/quazip
/mnt/4/android/qt$QT_VERSION/$QT_VERSION/android_armv7/bin/qmake ../../quazip
make
popd

pushd android
/mnt/4/android/qt$QT_VERSION/$QT_VERSION/android_armv7/bin/qmake
make
popd

pushd android
make install INSTALL_ROOT=../apk
popd

mkdir -p apk/assets/fonts/
cp data/search.db apk/assets/
cp data/amiri-regular.ttf apk/assets/fonts/
cp data/amiri-quran.ttf apk/assets/fonts/

/mnt/4/android/android-sdk-linux/tools//android update project --path apk \
    --target android-19 --name Quran

/mnt/4/android/qt$QT_VERSION/$QT_VERSION/android_armv7/bin/androiddeployqt \
    --input android/android-libQuran.so-deployment-settings.json \
    --output apk \
    --deployment bundled \
    --verbose

pushd apk
ant installd
popd
