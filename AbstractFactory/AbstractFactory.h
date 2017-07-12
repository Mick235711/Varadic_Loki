//
// Created by lee on 17/7/5.
//

#ifndef ABSTRACTFACTORY_ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_ABSTRACTFACTORY_H

#include "../TypeList/TypeList.h"
#include "../TypeList/HierarchyGenerator.h"
#include "FactoryUnit.h"

// AbstractFactory
// Defines an Abstract Factory interface starting from a typelist
template
    <
        typename TList,
        template<typename> class Unit = AbstractFactoryUnit
    >
class AbstractFactory : public TL::GenScatterHierarchy<TList, Unit>
{
public:
    typedef TList ProductList;
    template<typename T> T* Create()
    {
        Unit<T>& unit = *this;
        return unit.DoCreate(Type2Type<T>());
    }
};

// ConcreteFactory
// Implements an AbstractFactory interface
template
    <
        typename AbstractFact,
        template<typename, typename> class Creator = OpNewFactoryUnit,
        typename TList = typename AbstractFact::ProductList
    >
class ConcreteFactory : public TL::GenLinearHierarchy<typename TL::Reverse<TList>::type, Creator, AbstractFact>
{
public:
    typedef typename AbstractFact::ProductList ProductList;
    typedef TList ConcreteProductList;
};

#endif //ABSTRACTFACTORY_ABSTRACTFACTORY_H
