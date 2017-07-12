#include <iostream>
#include <vector>
#include <ctime>
#include "SmallObj/FixedAllocator.hpp"
#include "SmallObj/SmallObjAllocator.hpp"
#include "Singleton/SingletonHolder.hpp"
#include "SmallObj/SmallObject.hpp"

struct A
{
    int i;
    A() :i(0) {}
};

struct B : SmallObject<>
{
    int i;
    B() :i(0) {}
};

int main()
{
    std::clock_t b1, e1, b2, e2;
    int i;
    A* a;
    B* b;
    b1 = clock();
    for (i = 1; i <= 10000; i++) {a = new A; delete a;}
    e1 = clock();
    b2 = clock();
    for (i = 1; i <= 10000; i++) {b = new B; delete b;}
    e2 = clock();
    std::clock_t d1 = e1 - b1, d2 = e2 - b2;
    std::cout << "new: " << double(d1) / CLOCKS_PER_SEC << std::endl;
    std::cout << "SmallObject: " << double(d2) / CLOCKS_PER_SEC << std::endl;
    return 0;
}