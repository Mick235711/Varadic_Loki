cmake_minimum_required(VERSION 3.7)
project(AbstractFactory)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp AbstractFactory.h FactoryUnit.h)
add_executable(AbstractFactory ${SOURCE_FILES})