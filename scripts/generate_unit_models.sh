#!/bin/bash

files=( scenes/box.3d scenes/cone.3d scenes/plane.3d scenes/sphere.3d  scenes/torus.3d )
args=( "1 1 1 10"        "1 1 20 20"    "1"             "1 50 50"         "0.545 2.327 50 50")
for ((i = 0; i < ${#files[@]}; i++))
do
    echo -en "\033[32m"
    echo "${files[$i]}" "$(basename "${files[$i]}" .3d)" ${args[$i]}
    echo -en "\033[0m"
    ./target/release/generator/generator "${files[$i]}" "$(basename "${files[$i]}" .3d)" ${args[$i]}
done
echo -en "\033[32m"
echo scenes/piramid.3d cone 1 1 4 30
echo -en "\033[0m"
./target/release/generator/generator scenes/piramid.3d cone 1 1 4 30
echo -en "\033[32m"
echo scenes/teapot.3d patch scenes/teapot.patch 5
./target/release/generator/generator scenes/teapot.3d patch scenes/teapot.patch 5
echo -en "\033[0m"
