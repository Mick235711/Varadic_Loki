#include <iostream>
#include "SingletonHolder.cpp"

struct A
{
    A() {std::cout << "A::A()" << std::endl;}
    void fun() {std::cout << "A::fun()" << std::endl;}
    ~A() {std::cout << "A::~A()" << std::endl;}
};

struct B
{
    B() {std::cout << "B::B()" << std::endl;}
    void fun() {std::cout << "B::fun()" << std::endl;}
    ~B() {std::cout << "B::~B()" << std::endl;}
};

struct C
{
    C() {std::cout << "C::C()" << std::endl;}
    void fun() {std::cout << "C::fun()" << std::endl;}
    ~C() {std::cout << "C::~C()" << std::endl;}
};

struct D
{
    D() {std::cout << "D::D()" << std::endl;}
    void fun() {std::cout << "D::fun()" << std::endl;}
    ~D() {std::cout << "D::~D()" << std::endl;}
};

struct E
{
    E() {std::cout << "E::E()" << std::endl;}
    void fun() {std::cout << "E::fun()" << std::endl;}
    ~E() {std::cout << "E::~E()" << std::endl;}
};

typedef SingletonHolder<A> SHA;
typedef SingletonHolder<B, CreateUsingMalloc> SHB;
typedef SingletonHolder<C, CreateStatic, PhoenixSingleton> SHC;
typedef SingletonHolder<D, CreateUsingNew, SingletonWithLongevity> SHD;
typedef SingletonHolder<E, CreateStatic, SingletonWithLongevity> SHE;

DEFINE_GETLONGEVITY(D, 4);
DEFINE_GETLONGEVITY(E, 3);

int main()
{
    SHA::Instance().fun();
    SHB::Instance().fun();
    SHC::Instance().fun();
    SHD::Instance().fun();
    SHE::Instance().fun();
    return 0;
}