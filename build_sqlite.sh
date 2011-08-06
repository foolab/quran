#!/bin/sh

pushd sqlite

CFLAGS="-Os -DSQLITE_ENABLE_FTS4=1"  ./configure  --enable-readline=no  --enable-threadsafe=yes  --enable-shared=no  --enable-static=yes

make

popd