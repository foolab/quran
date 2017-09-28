DEPENDPATH += $$SQLITE_SRC_PATH/
INCLUDEPATH += $$SQLITE_SRC_PATH

PRE_TARGETDEPS += sqlite/.libs/libsqlite3.a
QMAKE_EXTRA_TARGETS += sqlite/.libs/libsqlite3.a

SQLITE_EXTRA_COMPILE_FLAGS +=

sqlite/.libs/libsqlite3.a.depends = $$SQLITE_SRC_PATH/configure
sqlite/.libs/libsqlite3.a.commands = mkdir -p sqlite/ && cd sqlite && \
                      CFLAGS=\"$$(CFLAGS) -DSQLITE_ENABLE_FTS4=1\" \
		      ../$$SQLITE_SRC_PATH/configure \
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
