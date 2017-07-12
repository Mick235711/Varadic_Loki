//
// Created by lee on 17/7/12.
//

#define BOOST_TEST_MODULE TestAssocVector
#include <boost/test/included/unit_test.hpp>
#include "AssocVector/AssocVector.h"
#include <utility>

BOOST_AUTO_TEST_SUITE(AssocVectorTest)

BOOST_AUTO_TEST_CASE(case1)
    {
        typedef AssocVector<std::string, int> MP;
        MP mp_int{{"Bob", 1}, {"Oh", 5}};
        mp_int.insert(std::make_pair("Alice", 2));
        BOOST_REQUIRE_GT(mp_int.count("Alice"), 0);
        BOOST_REQUIRE_EQUAL(mp_int["Oh"], 5);
        mp_int["Jack"] = 10;
        BOOST_REQUIRE_EQUAL(mp_int.size(), 4);
        BOOST_REQUIRE_EQUAL(mp_int.find("Bob")->second, 1);
    }

BOOST_AUTO_TEST_SUITE_END()
