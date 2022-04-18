#define BOOST_TEST_MODULE IPv4Address

#include "IPv4Address.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(bytes)
{
    IPv4Address addr("1.2.3.4");
    std::array<std::uint8_t, 4> expectedBytes{1, 2, 3, 4};

    auto actualBytes = addr.bytes();

    BOOST_TEST(actualBytes == expectedBytes);
}

BOOST_AUTO_TEST_CASE(indexOperator)
{
    IPv4Address addr("1.2.3.4");

    BOOST_CHECK_EQUAL(addr[0], 1);
    BOOST_CHECK_EQUAL(addr[1], 2);
    BOOST_CHECK_EQUAL(addr[2], 3);
    BOOST_CHECK_EQUAL(addr[3], 4);
}

BOOST_AUTO_TEST_CASE(greaterThanOperator)
{
    BOOST_TEST(IPv4Address("1.1.1.1") > IPv4Address("0.1.1.1"));
    BOOST_TEST(IPv4Address("1.1.1.1") > IPv4Address("1.0.1.1"));
    BOOST_TEST(IPv4Address("1.1.1.1") > IPv4Address("1.1.0.1"));
    BOOST_TEST(IPv4Address("1.1.1.1") > IPv4Address("1.1.1.0"));
}

BOOST_AUTO_TEST_CASE(toString)
{
    IPv4Address addr("1.2.3.4");

    auto actualResult = addr.toString();

    BOOST_CHECK_EQUAL(actualResult, "1.2.3.4");
}

BOOST_AUTO_TEST_CASE(outputStreamOperator)
{
    IPv4Address addr("1.2.3.4");
    std::ostringstream sout;

    sout << addr;

    BOOST_CHECK_EQUAL(sout.str(), "1.2.3.4");
}

BOOST_AUTO_TEST_CASE(tryToConstructFromInvalidAddress)
{
    IPv4Address addr("invalid address");
    std::array<std::uint8_t, 4> expectedBytes{0, 0, 0, 0};

    auto actualBytes = addr.bytes();

    BOOST_TEST(actualBytes == expectedBytes);
}
