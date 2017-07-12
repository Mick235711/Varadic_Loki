cmake_minimum_required(VERSION 3.7)
project(TypeList)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp TypeList.h HierarchyGenerator.h TypeManip.h)
add_executable(TypeList ${SOURCE_FILES})
