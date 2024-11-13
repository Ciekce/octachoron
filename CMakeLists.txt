cmake_minimum_required(VERSION 3.15)
project(octachoron)

set(CMAKE_CXX_STANDARD 20)

add_executable(octachoron src/main.cpp src/types.h src/core.h)
