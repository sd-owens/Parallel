cmake_minimum_required(VERSION 3.15)
project(p5_monteCarlo_CUDA)

# added for openMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -lm -fopenmp")

set(SOURCE_FILES monteCarloCUDA.cpp)
add_executable(monteCarloCUDA ${SOURCE_FILES})