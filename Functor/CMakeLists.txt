cmake_minimum_required(VERSION 3.7)
project(Functor)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Functor.hpp FunctorImpl.hpp FunctorHandler.hpp MemFnHandler.hpp BinderFirst.hpp BinderSecond.hpp FunctorChainer.hpp Functor.h FunctorImpl.h FunctorHandler.h MemFnHandler.h BinderFirst.h BinderSecond.h FunctorChainer.h)
add_executable(Functor ${SOURCE_FILES})
