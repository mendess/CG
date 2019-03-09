#!/bin/bash
build_dir="target"
mkdir -p $build_dir
cmake -S src -B $build_dir || exit 1
cd $build_dir || exit 1
make || exit 1
