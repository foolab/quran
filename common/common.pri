TEMPLATE = app
TARGET = n900-quran
DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/

DEFINES += VERSION=\\\"0.1\\\" DATA_DIR=\\\"/usr/share/n900-quran/\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp abstractquranview.cpp \
           position.cpp legal.cpp quranview.cpp logoprovider.cpp windowcontroller.cpp

HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h abstractquranview.h \
           position.h legal.h quranview.h logoprovider.h windowcontroller.h

target.path = /usr/bin/

INSTALLS += target
