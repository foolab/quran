DEPENDPATH += $$SQLITE_SRC_PATH/
INCLUDEPATH += $$SQLITE_SRC_PATH

PRE_TARGETDEPS += sqlite/.libs/libsqlite3.a
QMAKE_EXTRA_TARGETS += sqlite/.libs/libsqlite3.a

SQLITE_EXTRA_COMPILE_FLAGS +=

sqlite/.libs/libsqlite3.a.depends = $$SQLITE_SRC_PATH/configure
sqlite/.libs/libsqlite3.a.commands = mkdir -p sqlite/ && cd sqlite && \
                      CC=$${QMAKE_CC} \
                      CFLAGS=\"$${QMAKE_CFLAGS} -DSQLITE_ENABLE_FTS4=1 -fPIC -B$${QMAKE_LINK}\" \
                      LDFLAGS=\"$${QMAKE_LFLAGS}\" \
		      ../$$SQLITE_SRC_PATH/configure \
		      --enable-readline=no \
		      --enable-threadsafe=no \
		      --enable-shared=no \
		      --enable-static=yes \
		      --enable-dynamic-extensions=no \
		      --enable-fts4=yes \
		      $$SQLITE_EXTRA_COMPILE_FLAGS \
		      && make PROGRAMS=
# PROGRAMS= is used to disable sqlite3 binary because it does not link
# /usr/bin/ld: sqlite3-sqlite3.o: undefined reference to symbol 'pthread_join@@GLIBC_2.2.5'
# /usr/bin/ld: /lib/x86_64-linux-gnu/libpthread.so.0: error adding symbols: DSO missing from command line
# collect2: error: ld returned 1 exit status

LIBS += sqlite/.libs/libsqlite3.a

sqliteclean.commands = rm -rf sqlite
clean.depends += sqliteclean
distclean.depends += sqliteclean

QMAKE_EXTRA_TARGETS += clean distclean sqliteclean
