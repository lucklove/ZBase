#include "UnitTest.hh"
#include "Variant.hh"
#include <string>

TEST_CASE(varaint_test)
{
    Variant<int, std::string, const char*> v;
    v = 47;
    TEST_REQUIRE(v.is<int>());
    TEST_CHECK(v.get<int>() == 47);
    v = std::string{"string"};
    TEST_REQUIRE(v.is<std::string>());
    TEST_CHECK(v.get<std::string>() == "string");
    v = "const char*";
    TEST_REQUIRE(v.is<const char*>());
    TEST_CHECK(v.get<const char*>() == std::string{"const char*"});
}
