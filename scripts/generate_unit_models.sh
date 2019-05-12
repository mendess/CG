#!/bin/bash

files=( models/box.3d models/cone.3d models/plane.3d models/sphere.3d  models/torus.3d )
args=( "1 1 1 1"        "1 1 20 20"    "1"             "1 50 50"         "0.545 2.327 50 30")
for ((i = 0; i < ${#files[@]}; i++))
do
    echo -en "\033[32m"
    echo "${files[$i]}" "$(basename "${files[$i]}" .3d)" ${args[$i]}
    echo -en "\033[0m"
    ./target/release/generator/generator "${files[$i]}" "$(basename "${files[$i]}" .3d)" ${args[$i]}
done
echo -en "\033[32m"
echo models/piramid.3d cone 1 1 4 30
echo -en "\033[0m"
./target/release/generator/generator models/piramid.3d cone 1 1 4 30
echo -en "\033[32m"
echo models/teapot.3d patch models/teapot.patch 5
./target/release/generator/generator models/teapot.3d patch models/teapot.patch 5
echo -en "\033[0m"
