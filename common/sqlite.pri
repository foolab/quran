DEPENDPATH += ../sqlite/ sqlite/
INCLUDEPATH += ../sqlite/ sqlite/

PRE_TARGETDEPS += sqlite/.libs/libsqlite3.a
QMAKE_EXTRA_TARGETS += sqlite/.libs/libsqlite3.a

SQLITE_EXTRA_COMPILE_FLAGS +=

sqlite/.libs/libsqlite3.a.depends = ../sqlite/configure
sqlite/.libs/libsqlite3.a.commands = mkdir -p sqlite/ && cd sqlite && \
                      CFLAGS=\"$$(CFLAGS) -DSQLITE_ENABLE_FTS4=1\" \
		      ../../sqlite/configure \
		      --enable-readline=no \
		      --enable-threadsafe=no \
		      --enable-shared=no \
		      --enable-static=yes \
		      --enable-dynamic-extensions=no \
		      $$SQLITE_EXTRA_COMPILE_FLAGS \
		      && make

LIBS += sqlite/.libs/libsqlite3.a

sqliteclean.commands = rm -rf sqlite
clean.depends += sqliteclean
distclean.depends += sqliteclean

QMAKE_EXTRA_TARGETS += clean distclean sqliteclean
