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
