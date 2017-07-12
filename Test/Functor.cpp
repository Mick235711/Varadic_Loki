//
// Created by lee on 17/7/12.
//

#define BOOST_TEST_MODULE TestFunctor
#include <boost/test/included/unit_test.hpp>
#include "Functor/Functor.hpp"
#include "Functor/FunctorHandler.hpp"
#include "Functor/MemFnHandler.hpp"
#include "Functor/BinderFirst.hpp"
#include "Functor/BinderSecond.hpp"
#include "Functor/FunctorChainer.hpp"

BOOST_AUTO_TEST_SUITE(FunctorTest)

int val = 0;

// test functor
struct TestFunctor
{
    char operator()(int i, double d)
    {
        val = 1;
        return i == 3 ? 'a' : 'b';
    }
};

// test function
long TestFunction(long i, float d)
{
    val = 2;
    return i == 3 ? 'a' : 'b';
}

// test member function
struct TestMemFn
{
    short func(unsigned int i, int d)
    {
        val = 3;
        return i == 3 ? 'a' : 'b';
    }
};

// test chainer
void TestChain()
{
    val = 4;
}

BOOST_AUTO_TEST_CASE(case1)
    {
        TestFunctor f;
        TestMemFn mf;
        Functor<int, long long, long double> cmd(f), cmd2(TestFunction), cmd3(&mf, &TestMemFn::func);
        BOOST_REQUIRE_EQUAL(val, 0);
        BOOST_REQUIRE_EQUAL(cmd(3, 3.5), 'a');
        BOOST_REQUIRE_EQUAL(val, 1);
        BOOST_REQUIRE_EQUAL(cmd2(3, 3.5), 'a');
        BOOST_REQUIRE_EQUAL(val, 2);
        BOOST_REQUIRE_EQUAL(cmd3(3, 3.5), 'a');
        BOOST_REQUIRE_EQUAL(val, 3);
        BOOST_REQUIRE_EQUAL(cmd(4, 4.54), 'b');
        BOOST_REQUIRE_EQUAL(val, 1);
        BOOST_REQUIRE_EQUAL(cmd2(4, 4.5), 'b');
        BOOST_REQUIRE_EQUAL(val, 2);
        BOOST_REQUIRE_EQUAL(cmd3(4, 4.5), 'b');
        BOOST_REQUIRE_EQUAL(val, 3);
        Functor<int, long double> bound = BindFirst(cmd, 3), bound2 = BindSecond(cmd2, 4.5);
        BOOST_REQUIRE_EQUAL(bound(3.5), 'a');
        BOOST_REQUIRE_EQUAL(val, 1);
        BOOST_REQUIRE_EQUAL(bound2(4), 'b');
        BOOST_REQUIRE_EQUAL(val, 2);
        auto chained = Chain(TestChain, bound2);
        BOOST_REQUIRE_EQUAL(chained(4), 'b');
        BOOST_REQUIRE_EQUAL(val, 2);
    }

BOOST_AUTO_TEST_SUITE_END()
