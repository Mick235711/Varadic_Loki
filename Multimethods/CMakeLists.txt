cmake_minimum_required(VERSION 3.7)
project(Multimethods)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Dispatcher.h Casting.h)
add_executable(Multimethods ${SOURCE_FILES})