TEMPLATE = app

DEPENDPATH += . $$PWD/common
INCLUDEPATH += . $$PWD/common
VPATH += $$PWD/common

greaterThan(QT_MAJOR_VERSION, 4) {
  QMAKE_CXXFLAGS += -std=c++11
}

QT = core

