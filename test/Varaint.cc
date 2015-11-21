#include <boost/test/unit_test.hpp>
#include <string>
#include "Varaint.hh"

BOOST_AUTO_TEST_CASE(varaint_test)
{
    Variant<int, std::string, const char*> v;
    v = 47;
    BOOST_REQUIRE(v.is<int>());
    BOOST_CHECK(v.get<int>() == 47);
    v = std::string{"string"};
    BOOST_REQUIRE(v.is<std::string>());
    BOOST_CHECK(v.get<std::string>() == "string");
    v = "const char*";
    BOOST_REQUIRE(v.is<const char*>());
    BOOST_CHECK(v.get<const char*>() == std::string{"const char*"});
}
