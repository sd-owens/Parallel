cmake_minimum_required(VERSION 3.15)
project(p4_simd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -o3-lm -fopenmp")

set(SOURCE_FILES p4_simd.cpp)

add_executable(p4_SIMD ${SOURCE_FILES})