cmake_minimum_required(VERSION 3.7)
project(Loki)

set(CMAKE_CXX_STANDARD 11)

#set(SOURCE_FILES
#        AbstractFactory/AbstractFactory.h
##        AbstractFactory/CMakeLists.txt
#        AbstractFactory/FactoryUnit.h
#        AbstractFactory/main.cpp
#        AssocVector/AssocVector.h
##        AssocVector/CMakeLists.txt
#        AssocVector/main.cpp
#        Factory/CloneFactory.h
##        Factory/CMakeLists.txt
#        Factory/Factory.h
#        Factory/FactoryError.h
#        Factory/main.cpp
#        Functor/BinderFirst.cpp
#        Functor/BinderFirst.h
#        Functor/BinderSecond.cpp
#        Functor/BinderSecond.h
##        Functor/CMakeLists.txt
#        Functor/Functor.cpp
#        Functor/Functor.h
#        Functor/FunctorChainer.cpp
#        Functor/FunctorChainer.h
#        Functor/FunctorHandler.cpp
#        Functor/FunctorHandler.h
#        Functor/FunctorImpl.cpp
#        Functor/FunctorImpl.h
#        Functor/main.cpp
#        Functor/MemFnHandler.cpp
#        Functor/MemFnHandler.h
#        Multimethods/Casting.h
##        Multimethods/CMakeLists.txt
#        Multimethods/Dispatcher.h
#        Multimethods/main.cpp
##        Singleton/CMakeLists.txt
#        Singleton/Create.h
#        Singleton/Lifetime.h
#        Singleton/Longevity.cpp
#        Singleton/Longevity.h
#        Singleton/main.cpp
#        Singleton/SingletonHolder.cpp
#        Singleton/SingletonHolder.h
##        SmallObj/CMakeLists.txt
#        SmallObj/FixedAllocator.cpp
#        SmallObj/FixedAllocator.h
#        SmallObj/main.cpp
#        SmallObj/SmallObjAllocator.cpp
#        SmallObj/SmallObjAllocator.h
#        SmallObj/SmallObject.cpp
#        SmallObj/SmallObject.h
#        SmartPtr/Checking.h
##        SmartPtr/CMakeLists.txt
#        SmartPtr/Conversion.h
#        SmartPtr/main.cpp
#        SmartPtr/Ownership.h
#        SmartPtr/SmartPtr.h
#        SmartPtr/Storage.h
##        Threading/CMakeLists.txt
#        Threading/main.cpp
#        Threading/Threading.h
##        TypeInfo/CMakeLists.txt
#        TypeInfo/main.cpp
#        TypeInfo/TypeInfo.cpp
#        TypeInfo/TypeInfo.h
##        TypeList/CMakeLists.txt
#        TypeList/HierarchyGenerator.h
#        TypeList/main.cpp
#        TypeList/TypeList.h
#        TypeList/TypeManip.h
#        Visitor/CatchAll.h
##        Visitor/CMakeLists.txt
#        Visitor/CyclicVisitor.h
#        Visitor/main.cpp
#        Visitor/Visitable.h
#        Visitor/Visitor.h)

include_directories(.)

add_subdirectory(AbstractFactory)
add_subdirectory(AssocVector)
add_subdirectory(Factory)
add_subdirectory(Functor)
add_subdirectory(Multimethods)
add_subdirectory(Singleton)
add_subdirectory(SmallObj)
add_subdirectory(SmartPtr)
add_subdirectory(Threading)
add_subdirectory(TypeInfo)
add_subdirectory(TypeList)
add_subdirectory(Visitor)
add_subdirectory(Test)

enable_testing()
add_test(NAME MyTest COMMAND Test)
