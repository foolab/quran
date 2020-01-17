#!/bin/bash

set -e

QT_VERSION=5.13.2
ANDROID=arm64_v8a
ANDROID_API=21
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

#rm -rf apk

pushd android
$QT_DIR/android_$ANDROID/bin/qmake
make
popd

pushd android
make install INSTALL_ROOT=../apk
popd

$QT_DIR/android_$ANDROID/bin/androiddeployqt \
    --input android/android-libQuran.so-deployment-settings.json \
    --output apk \
    --deployment bundled \
    --android-platform android-${ANDROID_API} \
    --gradle \
    $ANT_ARG
