cmake_minimum_required(VERSION 3.7)
project(TypeInfo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp TypeInfo.cpp TypeInfo.h)
add_executable(TypeInfo ${SOURCE_FILES})