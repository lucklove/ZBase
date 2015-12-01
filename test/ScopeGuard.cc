#include "UnitTest.hh"
#include "ScopeGuard.hh"

TEST_CASE(block_guard)
{
    bool flag = false;
    {
        ScopeGuard s{[&]{ flag = true; }};
    }
    TEST_CHECK(flag);
}

TEST_CASE(throw_guard)
{
    bool flag = false;
    try
    {
        ScopeGuard s{[&]{ flag = true; }};
        throw "whatever";
        TEST_CHECK(false);
    }
    catch(...)
    {
    }
    TEST_CHECK(flag);
}

TEST_CASE(return_guard)
{
    bool flag = false;
    [&]
    {
        bool& f = flag;
        ScopeGuard s{[&]{ f = true; }};
        return;
    }();
    TEST_CHECK(flag);
}

TEST_CASE(dismiss_test)
{
    bool flag = true;
    {
        ScopeGuard s{[&]{ flag = false; }};
        s.dismiss();
    }
    TEST_CHECK(flag);
}
