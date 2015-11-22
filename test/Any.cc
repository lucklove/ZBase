#include "UnitTest.hh"
#include <iostream>
#include "Any.hh"

TEST_CASE(any_test)
{
    Any a;
    TEST_CHECK(a.is<void>());
    TEST_CHECK(a.isNull());
    a = 47;
    TEST_REQUIRE(a.is<int>());
    TEST_CHECK(a.cast<int>() == 47);
    a = "const char*";
    TEST_REQUIRE(a.is<const char*>()); 
    a = std::string{"string"};
    TEST_REQUIRE(a.is<std::string>());
    TEST_CHECK(a.cast<std::string>() == "string");
}
