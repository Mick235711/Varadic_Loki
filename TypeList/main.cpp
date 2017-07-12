#include <iostream>
#include "TypeList.h"
#include "HierarchyGenerator.h"

template<typename T>
struct Holder
{
    T value;
};

int main()
{
    std::cout << "Hello, World!" << std::endl;
    typedef TL::TypeList<int, long long, double, short, int> Types;
    typedef TL::GenScatterHierarchy<Types, Holder> typesHchy;
    typesHchy Hchy;
    TL::Field<long long>(Hchy).value = 1LL;
    TL::Field<2>(Hchy).value = 0.5;
    std::cout << TL::Field<double>(Hchy).value << std::endl;
    return 0;
}