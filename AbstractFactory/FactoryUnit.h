//
// Created by lee on 17/7/5.
//

#ifndef ABSTRACTFACTORY_FACTORYUNIT_H
#define ABSTRACTFACTORY_FACTORYUNIT_H

#include "TypeList/TypeManip.h"
#include <cassert>

// AbstractFactoryUnit
// The building block of an Abstract Factory
template<typename T>
class AbstractFactoryUnit
{
public:
    virtual T* DoCreate(Type2Type<T>) = 0;
    virtual ~AbstractFactoryUnit() = default;
};

// OpNewFactoryUnit
// Creates an object by invoking the new operator
template<typename ConcreteProduct, typename Base>
class OpNewFactoryUnit : public Base
{
    typedef typename Base::ProductList BaseProductList;

protected:
    typedef typename BaseProductList::Tail ProductList;
    
public:
    typedef typename BaseProductList::Head AbstractProduct;
    ConcreteProduct* DoCreate(Type2Type<AbstractProduct>)
    {
        return new ConcreteProduct;
    }
};

// PrototypeFactoryUnit
// Creates an object by cloning a prototype
template<typename ConcreteProduct, typename Base>
class PrototypeFactoryUnit : public Base
{
    typedef typename Base::ProductList BaseProductList;

protected:
    typedef typename BaseProductList::Tail ProductList;

public:
    typedef typename BaseProductList::Head AbstractProduct;
    
    PrototypeFactoryUnit(AbstractProduct* p = nullptr)
        : m_pPrototype(p)
    {}
    
    friend void DoGetPrototype(const PrototypeFactoryUnit& me, AbstractProduct*& pPrototype)
    {pPrototype = me.m_pPrototype;}
    
    friend void DoSetPrototype(PrototypeFactoryUnit& me, AbstractProduct* pObj)
    {me.m_pPrototype = pObj;}
    
    template<typename U>
    void GetPrototype(U*& p)
    {return DoGetPrototype(*this, p);}
    
    template<typename U>
    void SetPrototype(U* pObj)
    {DoSetPrototype(*this, pObj);}
    
    AbstractProduct* DoCreate(Type2Type<AbstractProduct>)
    {
        assert(m_pPrototype);
        return m_pPrototype->Clone();
    }

private:
    AbstractProduct* m_pPrototype;
};

#endif //ABSTRACTFACTORY_FACTORYUNIT_H
