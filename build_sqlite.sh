#!/bin/sh

mkdir -p sqlite
pushd sqlite

CFLAGS="-DSQLITE_ENABLE_FTS4=1" \
    ../../sqlite/configure \
    --enable-readline=no \
    --enable-threadsafe=no \
    --enable-shared=yes \
    --enable-static=no \
    --enable-dynamic-extensions=no \
    $@

make

popd
