cmake_minimum_required(VERSION 3.7)
project(Singleton)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Longevity.h Longevity.hpp Create.h Lifetime.h SingletonHolder.h SingletonHolder.hpp)
add_executable(Singleton ${SOURCE_FILES})