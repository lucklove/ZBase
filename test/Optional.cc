#include <boost/test/unit_test.hpp>
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

BOOST_AUTO_TEST_CASE(optional_test)
{
    BOOST_CHECK(func(true));   
    BOOST_CHECK(!func(false)); 
    BOOST_CHECK(*func(true) == 47); 
}
