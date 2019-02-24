#!/bin/bash

mkdir -p out
cmake -S . -B out
cd out || exit 1
make
cd ..
