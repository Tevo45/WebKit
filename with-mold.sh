#!/bin/sh -x

if [ -z $NUMBER_OF_PROCESSORS ]
then
    export NUMBER_OF_PROCESSORS=4
fi

echo Building with $NUMBER_OF_PROCESSORS cores

COMMONFLAGS="-DASSERT_ENABLED=1"

env \
    CFLAGS="$COMMONFLAGS" \
    CXXFLAGS="$COMMONFLAGS" \
    LDFLAGS=-B/usr/libexec/mold \
    build-webkit --cmakeargs="-DUSE_LD_LLD=OFF" "$@"
