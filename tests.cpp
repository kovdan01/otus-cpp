#define BOOST_TEST_MODULE tests

#include "lib.h"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version)
{
    auto [major, minor, patch] = version();
    BOOST_CHECK(major == 0);
    BOOST_CHECK(minor == 0);
    BOOST_CHECK(patch > 0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(test_message)

BOOST_AUTO_TEST_CASE(test_valid_message)
{
    BOOST_CHECK(message() == "Hello world!\n");
}

BOOST_AUTO_TEST_SUITE_END()
