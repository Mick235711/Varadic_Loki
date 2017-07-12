#include <iostream>
#include "../SmallObj/FixedAllocator.cpp"
#include "../SmallObj/SmallObjAllocator.cpp"
#include "../Singleton/SingletonHolder.cpp"
#include "SmartPtr.h"
#include <map>

struct A
{
    A() {std::cout << "A::A()" << std::endl;}
    void fun() {std::cout << "Hello" << std::endl;}
    ~A() {std::cout << "A::~A()" << std::endl;}
};

std::map<SmartPtr<A>, int> map_A;

void foo(A* a)
{
    std::cout << "Inside foobar(): ";
    a->fun();
}

void bar(SmartPtr<A> a)
{
    if (map_A.find(a) == map_A.end()) return;
    std::cout << "Mapped: " << map_A[a] << std::endl;
}

int main()
{
    SmartPtr<A> spa(new A);
    spa->fun();
    if (spa) {std::cout << "good" << std::endl;}
    foo(static_cast<A*>(spa));
    map_A[spa] = 1;
    bar(spa);
    map_A.clear();
    return 0;
}
