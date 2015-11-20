#include <boost/test/unit_test.hpp>
#include "AsyncWrapper.hh"

BOOST_AUTO_TEST_CASE(async_wrapper_test)
{
    asyncWrap([](auto callback)
    {   
        callback(1);
    }).then([](auto callback1, auto callback2, int x)
    {
        BOOST_CHECK(x == 1);
        callback1(2);
        BOOST_CHECK(callback2(3, 4) == 7); 
    }).then([](auto callback, int x)
    {
        BOOST_CHECK(x == 2);
        callback(5);
    }, [](auto callback, int y, int z)
    {
        BOOST_CHECK(y == 3);
        BOOST_CHECK(z == 4);
        callback(6);
        return y + z;
    }).then([](auto callback, int x)
    {
        BOOST_CHECK(x == 5 || x == 6);
    }).then([]
    {
        /** 该函数不应该被执行 */
        BOOST_CHECK(false);
    }).apply();
}
