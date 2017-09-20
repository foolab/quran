DEPENDPATH += ../libav/ libav/
INCLUDEPATH += ../libav/ libav/

PRE_TARGETDEPS += libav_build
QMAKE_EXTRA_TARGETS += libav_build

LIBAV_EXTRA_COMPILE_FLAGS +=

libav_build.depends = ../libav/configure
libav_build.commands = mkdir -p libav && cd libav && ../../libav/configure \
	       --disable-programs \
	       --disable-doc \
	       --disable-avdevice \
	       --disable-swscale \
	       --disable-network \
	       --disable-everything \
	       --enable-sram \
	       --enable-bsf=mp3_header_decompress \
	       --enable-demuxer=mp3 \
	       --enable-decoder=mp3 \
	       --enable-pic \
	       --enable-static \
	       --disable-shared \
	       $$LIBAV_EXTRA_COMPILE_FLAGS \
	       && make && cd .. && touch libav_build

LIBS += \
    -Wl,--start-group \
    libav/libavcodec/libavcodec.a \
    libav/libavformat/libavformat.a \
    libav/libavutil/libavutil.a \
    libav/libavfilter/libavfilter.a \
    libav/libavresample/libavresample.a \
    -Wl,--end-group

libavclean.commands = rm -rf libav libav_build
clean.depends += libavclean
distclean.depends += libavclean

QMAKE_EXTRA_TARGETS += clean distclean libavclean
