cmake_minimum_required(VERSION 3.5)

add_executable(generator
    main.cpp
    cone.cpp
    box.cpp
    cylinder.cpp
    plane.cpp
    sphere.cpp
    torus.cpp
    patches.cpp
)

target_link_libraries(
    generator
    common
)
