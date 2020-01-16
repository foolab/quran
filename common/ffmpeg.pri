DEPENDPATH += ../ffmpeg/ ffmpeg/$${QT_ARCH}
INCLUDEPATH += ../ffmpeg/ ffmpeg/$${QT_ARCH}

PRE_TARGETDEPS += ffmpeg_$${QT_ARCH}_build
QMAKE_EXTRA_TARGETS += ffmpeg_$${QT_ARCH}_build

FFMPEG_EXTRA_COMPILE_FLAGS +=

ffmpeg_$${QT_ARCH}_build.depends = ../ffmpeg/configure
ffmpeg_$${QT_ARCH}_build.commands = mkdir -p ffmpeg/$${QT_ARCH} && \
                        cd ffmpeg/$${QT_ARCH} && \
			../../../ffmpeg/configure \
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
	       && make && cd ../../ && touch ffmpeg_$${QT_ARCH}_build

LIBS += \
    -Wl,--start-group \
    ffmpeg/$${QT_ARCH}/libavcodec/libavcodec.a \
    ffmpeg/$${QT_ARCH}/libavformat/libavformat.a \
    ffmpeg/$${QT_ARCH}/libavutil/libavutil.a \
    ffmpeg/$${QT_ARCH}/libavfilter/libavfilter.a \
    ffmpeg/$${QT_ARCH}/libswresample/libswresample.a \
    -Wl,--end-group

ffmpegclean.commands = rm -rf ffmpeg/$${QT_ARCH} ffmpeg_$${QT_ARCH}_build
clean.depends += ffmpegclean
distclean.depends += ffmpegclean

QMAKE_EXTRA_TARGETS += clean distclean ffmpegclean
