#include "UnitTest.hh"
#include "Optional.hh"

Optional<int> func(bool f)
{
    if(f)           /**< 初始化Optional内的data */
    {
        return 47; 
    }
    else            /**< 不初始化 */
    {
        return {};
    }    
}

TEST_CASE(optional_test)
{
    TEST_CHECK(func(true));   
    TEST_CHECK(!func(false)); 
    TEST_CHECK(*func(true) == 47);
}

struct T
{
    void func(bool& f)
    {
        f = true;
    }

    void func(bool& f) const
    {
        f = true;
    }
};

TEST_CASE(point_operator_test)
{
    bool pass = false;
    Optional<T> opt = T{};
    opt->func(pass);
    TEST_CHECK(pass);
    pass = false;
    const Optional<T> c_opt = T{};
    c_opt->func(pass);   
    TEST_CHECK(pass);
}

TEST_CASE(init_test)
{
    Optional<int> opt1;
    const Optional<int> opt2;
    TEST_REQUIRE(!opt1);
    TEST_REQUIRE(!opt2);
    
    bool flag = false;
    try
    {
        *opt1;
        TEST_CHECK(false, "exception not emit");
    }
    catch(std::logic_error&)
    {
        flag = true;
    }
    TEST_CHECK(flag);

    flag = false;
    try
    {
        *opt2;
        TEST_CHECK(false, "exception not emit");
    }
    catch(std::logic_error&)
    {
        flag = true;
    }
    TEST_CHECK(flag);
}
