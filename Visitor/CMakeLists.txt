cmake_minimum_required(VERSION 3.7)
project(Visitor)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Visitor.h CatchAll.h Visitable.h CyclicVisitor.h)
add_executable(Visitor ${SOURCE_FILES})