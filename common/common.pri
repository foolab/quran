TEMPLATE = app
TARGET = quran
DEPENDPATH += . ../data/ ../common/
INCLUDEPATH += . ../data/ ../common/

DEFINES += VERSION=\\\"0.1\\\" DATA_DIR=\\\"/usr/share/quran/\\\"

SOURCES += bookmarks.cpp dataprovider.cpp numberformatter.cpp settings.cpp
HEADERS += bookmarks.h dataprovider.h numberformatter.h settings.h metadata.h

target.path = /usr/bin/

INSTALLS += target
