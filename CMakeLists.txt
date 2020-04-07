cmake_minimum_required(VERSION 3.15)
project(simple_openmp_clion)

# added for openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lm -fopenmp")

set(SOURCE_FILES p0_simple_openMP.cpp)
add_executable(simple_openmp_clion ${SOURCE_FILES})
