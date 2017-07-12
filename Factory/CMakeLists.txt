cmake_minimum_required(VERSION 3.7)
project(Factory)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp FactoryError.h Factory.h CloneFactory.h)
add_executable(Factory ${SOURCE_FILES})