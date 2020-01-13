VPATH += . ../data/ ../common/

TARGET = Quran

QT += qml quick sensors androidextras

CONFIG += android sles

equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
  EXTRA_CFLAGS="-march=armv7-a -mthumb"
  EXTRA_LDFLAGS="-march=armv7-a -mthumb -Xlinker --fix-cortex-a8"
  SQLITE_EXTRA_COMPILE_FLAGS += --host=arm-linux-androideabi
}

FFMPEG_EXTRA_COMPILE_FLAGS += --enable-cross-compile \
                              --target-os=android \
                              --disable-runtime-cpudetect \
                              --arch=arm \
                              --cpu=cortex-a8 \
                              --cc=\"$${QMAKE_CC} $${QMAKE_CFLAGS}\" \
                              --ld=\"$${QMAKE_LINK} $${QMAKE_LFLAGS}\" \
                              --nm=\"$${QMAKE_NM}\" \
                              --strip=\"$${QMAKE_STRIP}\" \
                              --extra-cflags=\"$$EXTRA_CFLAGS\" \
                              --extra-ldflags=\"$$EXTRA_LDFLAGS\"

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
           mediaservice.cpp \
           service.cpp \
           binder.cpp \
           intent.cpp \
           bundle.cpp \
           parcel.cpp \
           sqlite-ndk/sources/sqlite3ndk.cpp

HEADERS += fsmonitor.h \
           audiopolicy.h \
           androidsupport.h \
           keyfilter.h \
           mediaservice.h \
           service.h \
           binder.h \
           intent.h \
           bundle.h \
           parcel.h \
           sqlite-ndk/sources/sqlite3ndk.h

oboe {
INCLUDEPATH += oboe/src oboe/include

SOURCES += \
           oboe/src/aaudio/AAudioLoader.cpp \
           oboe/src/aaudio/AudioStreamAAudio.cpp \
           oboe/src/common/AudioSourceCaller.cpp \
           oboe/src/common/AudioStream.cpp \
           oboe/src/common/AudioStreamBuilder.cpp \
           oboe/src/common/DataConversionFlowGraph.cpp \
           oboe/src/common/FilterAudioStream.cpp \
           oboe/src/common/FixedBlockAdapter.cpp \
           oboe/src/common/FixedBlockReader.cpp \
           oboe/src/common/FixedBlockWriter.cpp \
           oboe/src/common/LatencyTuner.cpp \
           oboe/src/common/SourceFloatCaller.cpp \
           oboe/src/common/SourceI16Caller.cpp \
           oboe/src/common/Utilities.cpp \
           oboe/src/common/QuirksManager.cpp \
           oboe/src/fifo/FifoBuffer.cpp \
           oboe/src/fifo/FifoController.cpp \
           oboe/src/fifo/FifoControllerBase.cpp \
           oboe/src/fifo/FifoControllerIndirect.cpp \
           oboe/src/flowgraph/FlowGraphNode.cpp \
           oboe/src/flowgraph/ClipToRange.cpp \
           oboe/src/flowgraph/ManyToMultiConverter.cpp \
           oboe/src/flowgraph/MonoToMultiConverter.cpp \
           oboe/src/flowgraph/RampLinear.cpp \
           oboe/src/flowgraph/SampleRateConverter.cpp \
           oboe/src/flowgraph/SinkFloat.cpp \
           oboe/src/flowgraph/SinkI16.cpp \
           oboe/src/flowgraph/SinkI24.cpp \
           oboe/src/flowgraph/SourceFloat.cpp \
           oboe/src/flowgraph/SourceI16.cpp \
           oboe/src/flowgraph/SourceI24.cpp \
           oboe/src/flowgraph/resampler/IntegerRatio.cpp \
           oboe/src/flowgraph/resampler/LinearResampler.cpp \
           oboe/src/flowgraph/resampler/MultiChannelResampler.cpp \
           oboe/src/flowgraph/resampler/PolyphaseResampler.cpp \
           oboe/src/flowgraph/resampler/PolyphaseResamplerMono.cpp \
           oboe/src/flowgraph/resampler/PolyphaseResamplerStereo.cpp \
           oboe/src/flowgraph/resampler/SincResampler.cpp \
           oboe/src/flowgraph/resampler/SincResamplerStereo.cpp \
           oboe/src/opensles/AudioInputStreamOpenSLES.cpp \
           oboe/src/opensles/AudioOutputStreamOpenSLES.cpp \
           oboe/src/opensles/AudioStreamBuffered.cpp \
           oboe/src/opensles/AudioStreamOpenSLES.cpp \
           oboe/src/opensles/EngineOpenSLES.cpp \
           oboe/src/opensles/OpenSLESUtilities.cpp \
           oboe/src/opensles/OutputMixerOpenSLES.cpp \
           oboe/src/common/StabilizedCallback.cpp \
           oboe/src/common/Trace.cpp \
           oboe/src/common/Version.cpp
}

LIBS += -landroid -lOpenSLES
ANDROID_PACKAGE_SOURCE_DIR = android/apk

apk_dir.path = /
apk_dir.files = apk/*

fonts.path = /assets/fonts/
fonts.files = ../data/amiri-regular.ttf ../data/amiri-quran.ttf

search.path = /assets/
search.files = ../data/search.db

INSTALLS += apk_dir fonts search
