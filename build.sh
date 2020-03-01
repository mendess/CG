#!/bin/bash
debug_build="debug"
release_build="release"
mkdir target
mkdir -p $debug_build
mkdir -p $release_build
cd target
if [ "$1" = "-g" ]
then
    cmake .. -DCMAKE_BUILD_TYPE=Debug -B $debug_build || exit 1
    cd $debug_build || exit 1
    make || exit 3
else
    cmake .. -DCMAKE_BUILD_TYPE=Release -B $release_build || exit 1
    cd $release_build || exit 1
    make || exit 2
fi
