cmake_minimum_required(VERSION 3.7)
project(SmallObj)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp FixedAllocator.hpp FixedAllocator.h SmallObjAllocator.hpp SmallObjAllocator.h SmallObject.hpp SmallObject.h)
add_executable(SmallObj ${SOURCE_FILES})