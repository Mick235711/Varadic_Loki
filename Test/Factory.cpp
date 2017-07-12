//
// Created by lee on 17/7/12.
//

#define BOOST_TEST_MODULE TestFactory

#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include "TypeInfo/TypeInfo.hpp"
#include "Functor/Functor.hpp"
#include "Functor/FunctorHandler.hpp"
#include "Singleton/SingletonHolder.hpp"
#include "Factory/Factory.h"
#include "Factory/CloneFactory.h"

BOOST_AUTO_TEST_SUITE(FactoryTest)
    
    struct A
    {
        virtual std::string fun() {return "A";}
    };
    
    struct B : public A
    {
        std::string fun() {return "B";}
    };
    
    typedef SingletonHolder<Factory<A, std::string, Functor<A*>>> AFactory;
    typedef SingletonHolder<CloneFactory<B>> BCloneFactory;
    
    struct MyCreator
    {
        A *operator()()
        {
            return new A;
        }
    };
    
    B* CreateB()
    {
        return new B;
    }
    B* CloneB(const B* b)
    {
        return new B(*b);
    }
    
BOOST_AUTO_TEST_CASE(case1)
    {
        AFactory::Instance().Register("MyCreator()()", MyCreator());
        AFactory::Instance().Register("CreateB()", CreateB);
        A* pA = AFactory::Instance().CreateObject("CreateB()");
        try
        {
            if (AFactory::Instance().Unregister("MyCreator()()"))
            {
                A *pA2 = AFactory::Instance().CreateObject("MyCreator()()");
                BOOST_REQUIRE_EQUAL(pA2->fun(), "A");
            }
        }
        catch(Factory<A, std::string, Functor<A*>>::Exception& e)
        { BOOST_REQUIRE_EQUAL(e.what(), "Unknown Type"); }
        BCloneFactory::Instance().Register(typeid(B), CloneB);
        A* pB = BCloneFactory::Instance().CreateObject(new B);
        BOOST_REQUIRE_EQUAL(pA->fun(), "B");
        BOOST_REQUIRE_EQUAL(pB->fun(), "B");
    }
    
BOOST_AUTO_TEST_SUITE_END()
