cmake_minimum_required(VERSION 3.9)

project(globalProject)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic")

find_package(GLEW 2.0 REQUIRED)

add_subdirectory(src/generator)
add_subdirectory(src/engine)
add_subdirectory(src/common)
target_include_directories(generator PUBLIC include)
target_include_directories(engine PUBLIC include)
target_include_directories(common PUBLIC include)

