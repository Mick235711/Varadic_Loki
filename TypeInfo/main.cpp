#include <iostream>
#include <map>
#include "TypeInfo/TypeInfo.hpp"

struct B{};
struct A{};

int main()
{
    std::map<TypeInfo, int> mp_ti{{typeid(double), 1}, {typeid(char), 4}, {typeid(A), 3}};
    mp_ti[typeid(int)] = 2;
    mp_ti[typeid(B)] = 5;
    for (auto i : mp_ti)
        std::cout << i.first.name() << ' ' << i.second << std::endl;
    return 0;
}
