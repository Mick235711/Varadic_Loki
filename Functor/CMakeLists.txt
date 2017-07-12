cmake_minimum_required(VERSION 3.7)
project(Functor)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Functor.cpp Functor.h FunctorImpl.cpp FunctorImpl.h FunctorHandler.cpp FunctorHandler.h MemFnHandler.cpp MemFnHandler.h BinderFirst.cpp BinderFirst.h BinderSecond.cpp BinderSecond.h FunctorChainer.cpp FunctorChainer.h)
add_executable(Functor ${SOURCE_FILES})