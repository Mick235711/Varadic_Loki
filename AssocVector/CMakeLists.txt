cmake_minimum_required(VERSION 3.7)
project(AssocVector)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp AssocVector.h)
add_executable(AssocVector ${SOURCE_FILES})