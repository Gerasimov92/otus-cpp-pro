#define BOOST_TEST_MODULE HelloWorldTestModule

#include "HelloWorld.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(HelloWorldTest)

BOOST_AUTO_TEST_CASE(greet)
{
    HelloWorld world;

    auto result = world.greet();

    BOOST_CHECK_EQUAL(result, "Hello World!");
}

BOOST_AUTO_TEST_SUITE_END()
