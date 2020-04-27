cmake_minimum_required(VERSION 3.15)
project(p3_farmVille)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -o3-lm -fopenmp")

set(SOURCE_FILES p3_farmVille.cpp)

add_executable(p3_farmVille ${SOURCE_FILES})