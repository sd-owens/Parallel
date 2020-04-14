cmake_minimum_required(VERSION 3.15)
project(p1_monteCarlo)

# added for openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -lm -fopenmp")

set(SOURCE_FILES monteCarlo.cpp)
add_executable(monteCarlo ${SOURCE_FILES})