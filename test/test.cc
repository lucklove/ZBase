#define TEST_MAIN
#include "UnitTest.hh"

struct T : std::logic_error
{
    using logic_error::logic_error;
};

TEST_CASE(test1)
{
    throw T{"llll"};
    *(int *)0 = 0;
}

TEST_CASE(test2)
{
    throw std::logic_error{"hh"};   
}
