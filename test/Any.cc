#include <boost/test/unit_test.hpp>
#include <iostream>
#include "Any.hh"

BOOST_AUTO_TEST_CASE(any_test)
{
    Any a;
    BOOST_CHECK(a.is<void>());
    BOOST_CHECK(a.isNull());
    a = 47;
    BOOST_REQUIRE(a.is<int>());
    BOOST_CHECK(a.cast<int>() == 47);
    a = "const char*";
    BOOST_REQUIRE(a.is<const char*>()); 
    a = std::string{"string"};
    BOOST_REQUIRE(a.is<std::string>());
    BOOST_CHECK(a.cast<std::string>() == "string");
}
