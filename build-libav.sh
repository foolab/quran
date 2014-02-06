#!/bin/sh

mkdir -p libav

pushd libav

 ../../libav/configure --disable-programs --disable-doc  --disable-avdevice --disable-swscale --disable-network  --disable-everything --enable-sram --enable-bsf=mp3_header_decompress --enable-demuxer=mp3 --enable-decoder=mp3 --enable-pic

make

popd
