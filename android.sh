#!/bin/bash

set -e

QT_VERSION=5.13.2
ANDROID=armv7
ANDROID_API=21
ANDROID_TARGET=armv7a-linux-androideabi${ANDROID_API}
ANT_ARG=debug

show_help() {
    echo "Usage: $0 <installd|installr|debug|release>"
    exit 1
}

if [ $# -gt 0 ]; then
    case $1 in
	help)
	    show_help
	    ;;
	installd)
	    ANT_ARG=--reinstall
	    ;;
	installr)
	    ANT_ARG="--release --install"
	    ;;
	debug)
	    ANT_ARG=
	    ;;
	release)
	    ANT_ARG=--release
	    ;;
	*)
	    show_help
	    ;;
    esac
fi

QT_DIR=/home/mohammed/Qt/$QT_VERSION/

# Needed for Qt
export ANDROID_NDK_ROOT=/home/mohammed/android/ndk-bundle/
export ANDROID_SDK_ROOT=/home/mohammed/android/

export TOOLCHAIN=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin/

export ANDROID_CFLAGS="-march=armv7-a -mthumb"
export ANDROID_LDFLAGS="-march=armv7-a -mthumb -Xlinker --fix-cortex-a8"

export CC=${TOOLCHAIN}/armv7a-linux-androideabi21-clang
export LD=${TOOLCHAIN}/armv7a-linux-androideabi21-clang
export STRIP=${TOOLCHAIN}/arm-linux-androideabi-strip
export NM=${TOOLCHAIN}/arm-linux-androideabi-nm
export PATH=${TOOLCHAIN}:${PATH}
#rm -rf apk

pushd android
$QT_DIR/android_$ANDROID/bin/qmake
make
popd

pushd android
make install INSTALL_ROOT=../apk
popd

mkdir -p apk/assets/fonts/
cp data/search.db apk/assets/
cp data/amiri-regular.ttf apk/assets/fonts/
cp data/amiri-quran.ttf apk/assets/fonts/

$QT_DIR/android_$ANDROID/bin/androiddeployqt \
    --input android/android-libQuran.so-deployment-settings.json \
    --output apk \
    --deployment bundled \
    --android-platform android-${ANDROID_API} \
    --gradle \
    $ANT_ARG
