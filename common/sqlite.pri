DEPENDPATH += ../sqlite/ sqlite/
INCLUDEPATH += ../sqlite/ sqlite/

PRE_TARGETDEPS += sqlite_build
QMAKE_EXTRA_TARGETS += sqlite_build

SQLITE_EXTRA_COMPILE_FLAGS +=

sqlite_build.depends = ../sqlite/configure
sqlite_build.commands = mkdir -p sqlite/ && cd sqlite && \
                      CFLAGS=\"$$(CFLAGS) -DSQLITE_ENABLE_FTS4=1\" \
		      ../../sqlite/configure \
		      --enable-readline=no \
		      --enable-threadsafe=no \
		      --enable-shared=no \
		      --enable-static=yes \
		      --enable-dynamic-extensions=no \
		      $$SQLITE_EXTRA_COMPILE_FLAGS \
		      && make && cd .. && touch sqlite_build

LIBS += sqlite/.libs/libsqlite3.a

sqliteclean.commands = rm -rf sqlite sqlite_build
clean.depends += sqliteclean
distclean.depends += sqliteclean

QMAKE_EXTRA_TARGETS += clean distclean sqliteclean
