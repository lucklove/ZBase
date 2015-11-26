#include "UnitTest.hh"
#include "Any.hh"
#include <iostream>
#include <functional>

TEST_CASE(any_basic_test)
{
    Any a;
    TEST_CHECK(a.is<void>());
    TEST_CHECK(a.isNull());
    a = 47;
    TEST_REQUIRE(a.is<int>());
    TEST_CHECK(a.get<int>() == 47);
    a = "const char*";
    TEST_REQUIRE(a.is<const char*>()); 
    a = std::string{"string"};
    TEST_REQUIRE(a.is<std::string>());
    TEST_CHECK(a.get<std::string>() == "string");
}

TEST_CASE(any_functional_test)
{
    bool check_flag = false;
    Any a = std::function<void()>([&]{ check_flag = true; });
    a.get<std::function<void()>>()();
    TEST_CHECK(check_flag);
}
