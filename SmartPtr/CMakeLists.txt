cmake_minimum_required(VERSION 3.7)
project(SmartPtr)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Storage.h Ownership.h Conversion.h Checking.h SmartPtr.h)
add_executable(SmartPtr ${SOURCE_FILES})