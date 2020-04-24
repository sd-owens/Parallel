cmake_minimum_required(VERSION 3.15)
project(p2_superQuadratic)

# added for openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -lm -fopenmp")

set(SOURCE_FILES superQuadratic.cpp)

add_executable(superQuadratic ${SOURCE_FILES})