#!/bin/bash
debug_build="target/debug"
release_build="target/release"
mkdir -p $debug_build
mkdir -p $release_build
cmake -DCMAKE_BUILD_TYPE=Debug -S src -B $debug_build || exit 1
cmake -DCMAKE_BUILD_TYPE=Releas -S src -B $release_build || exit 1
cd $release_build || exit 1
make || exit 2
cd ../.. || exit 2
cd $debug_build || exit 1
make || exit 3
