#!/bin/bash
params=( "10" "20 30 40 6" "10 100 200" "10 6 100 200" )
figures=( "plane" "box" "sphere" "cone" )

for ((i = 0; i < ${#figures[@]}; i++))
do
    figure=${figures[i]}
    param=${params[i]}
    echo -e "\033[33mvalgrind --leak-check=full --show-reachable=no --show-leak-kinds=all 'generator $figure.3d $figure $param'\033[0m"
    valgrind --leak-check=full --show-reachable=no --show-leak-kinds=all \
        target/release/generator/generator /tmp/file "$figure" $param

done

valgrind --leak-check=full --show-reachable=no --show-leak-kinds=all \
    target/release/engine/engine scenes/sistemasolar.xml -d
