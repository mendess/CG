#!/bin/bash
debug_build="target/debug"
release_build="target/release"
mkdir -p $debug_build
mkdir -p $release_build
if [ "$1" = "-g" ]
then
    cmake -Wno-dev -DCMAKE_BUILD_TYPE=Debug -S src -B $debug_build || exit 1
    cd $debug_build || exit 1
    make || exit 3
else
    cmake -Wno-dev -DCMAKE_BUILD_TYPE=Release -S src -B $release_build || exit 1
    cd $release_build || exit 1
    make || exit 2
fi
