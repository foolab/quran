#!/bin/bash
set -e

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

QT_VERSION=5.9.1
QT_DIR=/home/mohammed/mnt/4/android/qt$QT_VERSION/$QT_VERSION/

# Needed for Qt
export ANDROID_NDK_ROOT=/home/mohammed/mnt/4/android/android-ndk-r15c/
export ANDROID_SDK_ROOT=/home/mohammed/mnt/4/android/android-sdk-linux/

export TOOLCHAIN=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/
export SYSROOT=$ANDROID_NDK_ROOT/platforms/android-9/arch-arm/
export CFLAGS=--sysroot=$SYSROOT
export PATH=$TOOLCHAIN:$PATH

rm -rf apk

pushd android
$QT_DIR/android_armv7/bin/qmake
make
popd

pushd android
make install INSTALL_ROOT=../apk
popd

mkdir -p apk/assets/fonts/
cp data/search.db apk/assets/
cp data/amiri-regular.ttf apk/assets/fonts/
cp data/amiri-quran.ttf apk/assets/fonts/

$QT_DIR/android_armv7/bin/androiddeployqt \
    --input android/android-libQuran.so-deployment-settings.json \
    --output apk \
    --deployment bundled \
    --verbose \
    $ANT_ARG
