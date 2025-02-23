cmake_minimum_required(VERSION 3.30)
project(Lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab2 task1.cpp
        task2.cpp)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})