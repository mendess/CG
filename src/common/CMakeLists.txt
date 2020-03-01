cmake_minimum_required(VERSION 3.5)

# Project Name
add_library(common point.cpp util.cpp)
target_include_directories(common PUBLIC ../../include/common)
