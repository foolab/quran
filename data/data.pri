TEMPLATE = app

DEPENDPATH += . $$PWD/../common
INCLUDEPATH += . $$PWD/../common
VPATH += $$PWD/../common

CONFIG += c++11

QT = core

QMAKE_CLEAN += $$POST_OUT

post.target = $$POST_OUT
post.commands = $$POST_COMMAND
post.depends = $$TARGET

gen.depends = post

QMAKE_EXTRA_TARGETS += post gen
