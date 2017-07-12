#include <iostream>
#include "TypeInfo/TypeInfo.hpp"
#include "Functor/Functor.hpp"
#include "Functor/FunctorHandler.hpp"
#include "Singleton/SingletonHolder.hpp"
#include "Factory/Factory.h"
#include "Factory/CloneFactory.h"

struct A
{
    A() {std::cout << "A::A()" << std::endl;}
    ~A() {std::cout << "A::~A()" << std::endl;}
};

struct B : public A
{
    B() {std::cout << "B::B()" << std::endl;}
    ~B() {std::cout << "B::~B()" << std::endl;}
};

typedef SingletonHolder<Factory<A, std::string, Functor<A*>>> AFactory;
typedef SingletonHolder<CloneFactory<B>> BCloneFactory;

struct MyCreator
{
    A* operator()()
    {
        std::cout << "MyCreator()()" << std::endl;
        return new A;
    }
};

B* CreateB() {std::cout << "CreateB()" << std::endl; return new B;}
B* CloneB(const B* b) {std::cout << "CloneB()" << std::endl; return new B(*b);}

int main()
{
    AFactory::Instance().Register("MyCreator()()", MyCreator());
    AFactory::Instance().Register("CreateB()", CreateB);
    A* pA = AFactory::Instance().CreateObject("CreateB()");
    A* pA2;
    try
    {
        if (AFactory::Instance().Unregister("MyCreator()()"))
            pA2 = AFactory::Instance().CreateObject("MyCreator()()");
    }
    catch(Factory<A, std::string, Functor<A*>>::Exception& e)
    {std::cout << e.what() << std::endl;}
    BCloneFactory::Instance().Register(typeid(B), CloneB);
    B* pB = BCloneFactory::Instance().CreateObject(new B);
    return 0;
}