DEPENDPATH += ../ffmpeg/ ffmpeg/
INCLUDEPATH += ../ffmpeg/ ffmpeg/

PRE_TARGETDEPS += ffmpeg_build
QMAKE_EXTRA_TARGETS += ffmpeg_build

FFMPEG_EXTRA_COMPILE_FLAGS +=

ffmpeg_build.depends = ../ffmpeg/configure
ffmpeg_build.commands = mkdir -p ffmpeg && cd ffmpeg && ../../ffmpeg/configure \
               --cc=\"$${QMAKE_CC} $${QMAKE_CFLAGS}\" \
               --ld=\"$${QMAKE_LINK} $${QMAKE_LFLAGS}\" \
               --nm=\"$${QMAKE_NM}\" \
               --strip=\"$${QMAKE_STRIP}\" \
	       --ranlib=\"$${QMAKE_RANLIB}\" \
               --extra-cflags=\"$$EXTRA_CFLAGS\" \
               --extra-ldflags=\"$$EXTRA_LDFLAGS\" \
	       --disable-programs \
	       --disable-doc \
	       --disable-htmlpages \
	       --disable-manpages \
	       --disable-podpages \
	       --disable-txtpages \
	       --disable-avdevice \
	       --disable-swscale \
	       --disable-network \
	       --disable-everything \
	       --enable-bsf=mp3_header_decompress \
	       --enable-demuxer=mp3 \
	       --enable-decoder=mp3 \
	       --disable-avresample \
	       --enable-swresample \
	       --enable-pic \
	       --enable-static \
	       --disable-shared \
	       --disable-debug \
               --disable-cuda-llvm \
	       --disable-v4l2-m2m \
	       --disable-armv5te \
	       --disable-armv6 \
	       --disable-armv6t2 \
	       $$FFMPEG_EXTRA_COMPILE_FLAGS \
	       && make && cd .. && touch ffmpeg_build

LIBS += \
    -Wl,--start-group \
    ffmpeg/libavcodec/libavcodec.a \
    ffmpeg/libavformat/libavformat.a \
    ffmpeg/libavutil/libavutil.a \
    ffmpeg/libavfilter/libavfilter.a \
    ffmpeg/libswresample/libswresample.a \
    -Wl,--end-group

ffmpegclean.commands = rm -rf ffmpeg ffmpeg_build
clean.depends += ffmpegclean
distclean.depends += ffmpegclean

QMAKE_EXTRA_TARGETS += clean distclean ffmpegclean
