cmake_minimum_required(VERSION 3.7)
project(Test)

set(CMAKE_CXX_STANDARD 11)

add_executable(TestAbstractFactory AbstractFactory.cpp)
add_executable(TestAssocVector AssocVector.cpp)

add_custom_target(Test)
add_dependencies(Test TestAbstractFactory TestAssocVector)
