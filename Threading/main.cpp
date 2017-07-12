//
// Created by lee on 17/6/30.
//

#include "Threading.h"
#include <iostream>

int main()
{
    //typedef SingleThreaded<int> dt;
    //typedef ClassLevelLockable<int> dt;
    typedef ObjectLevelLockable<int> dt;
    typedef dt::IntType IntType;
    typedef dt::Lock Lock;
    dt DT;
    Lock* l = new Lock(DT);
    //Lock* l = new Lock();
    IntType a(2), b(3), c;
    dt::AtomicAdd(a, b);
    dt::AtomicAssign(c, a);
    std::cout << c << std::endl;
    delete l;
    return 0;
}
