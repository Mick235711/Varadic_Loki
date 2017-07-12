cmake_minimum_required(VERSION 3.7)
project(Threading)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES Threading.h main.cpp main.cpp)
add_executable(Threading ${SOURCE_FILES})