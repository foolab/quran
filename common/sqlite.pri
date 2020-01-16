DEPENDPATH += $$SQLITE_SRC_PATH
INCLUDEPATH += $$SQLITE_SRC_PATH

PRE_TARGETDEPS += sqlite_$${QT_ARCH}_build
QMAKE_EXTRA_TARGETS += sqlite_$${QT_ARCH}_build

SQLITE_EXTRA_COMPILE_FLAGS +=

sqlite_$${QT_ARCH}_build.depends = $$SQLITE_SRC_PATH/configure
sqlite_$${QT_ARCH}_build.commands = mkdir -p sqlite/$${QT_ARCH} && \
                        cd sqlite/$${QT_ARCH} && \
                      CC=$${QMAKE_CC} \
                      STRIP=\"$${QMAKE_STRIP}\" \
                      RANLIB=\"$${QMAKE_RANLIB}\" \
                      CFLAGS=\"$${QMAKE_CFLAGS} -DSQLITE_ENABLE_FTS4=1 -fPIC -B$${QMAKE_LINK}\" \
                      LDFLAGS=\"$${QMAKE_LFLAGS}\" \
		      ../../$$SQLITE_SRC_PATH/configure \
		      --enable-readline=no \
		      --enable-threadsafe=no \
		      --enable-shared=no \
		      --enable-static=yes \
		      --enable-dynamic-extensions=no \
		      --enable-fts4=yes \
		      $$SQLITE_EXTRA_COMPILE_FLAGS \
		      && make PROGRAMS= && cd ../../ && touch sqlite_$${QT_ARCH}_build
# PROGRAMS= is used to disable sqlite3 binary because it does not link
# /usr/bin/ld: sqlite3-sqlite3.o: undefined reference to symbol 'pthread_join@@GLIBC_2.2.5'
# /usr/bin/ld: /lib/x86_64-linux-gnu/libpthread.so.0: error adding symbols: DSO missing from command line
# collect2: error: ld returned 1 exit status

LIBS += sqlite/$${QT_ARCH}/.libs/libsqlite3.a

sqliteclean.commands = rm -rf sqlite/$${QT_ARCH} sqlite_$${QT_ARCH}_build
clean.depends += sqliteclean
distclean.depends += sqliteclean

QMAKE_EXTRA_TARGETS += clean distclean sqliteclean
