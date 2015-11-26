#include "UnitTest.hh"
#include "function_traits.hh"

int normal_function(bool, char, int)
{
    return {};
}

struct ClassType
{
    int member_function(bool, char, int);
    int operator()(bool, char, int);
};

TEST_CASE(function_traits_on_normal_function)
{
    TEST_CHECK(function_traits<decltype(normal_function)>::arity == 3);
    TEST_CHECK(typeid(function_traits<decltype(normal_function)>::function_type) == typeid(int(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(normal_function)>::return_type) == typeid(int));
    TEST_CHECK(typeid(function_traits<decltype(normal_function)>::stl_function_type) == typeid(std::function<int(bool, char, int)>));
    TEST_CHECK(typeid(function_traits<decltype(normal_function)>::pointer) == typeid(int(*)(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(normal_function)>::args<0>::type) == typeid(bool));
    TEST_CHECK(typeid(function_traits<decltype(normal_function)>::args<1>::type) == typeid(char));
    TEST_CHECK(typeid(function_traits<decltype(normal_function)>::args<2>::type) == typeid(int));
}

TEST_CASE(function_traits_on_function_pointer)
{
    int (*function_pointer)(bool, char, int) = normal_function;
    TEST_CHECK(function_traits<decltype(function_pointer)>::arity == 3);
    TEST_CHECK(typeid(function_traits<decltype(function_pointer)>::function_type) == typeid(int(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(function_pointer)>::return_type) == typeid(int));
    TEST_CHECK(typeid(function_traits<decltype(function_pointer)>::stl_function_type) == typeid(std::function<int(bool, char, int)>));
    TEST_CHECK(typeid(function_traits<decltype(function_pointer)>::pointer) == typeid(int(*)(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(function_pointer)>::args<0>::type) == typeid(bool));
    TEST_CHECK(typeid(function_traits<decltype(function_pointer)>::args<1>::type) == typeid(char));
    TEST_CHECK(typeid(function_traits<decltype(function_pointer)>::args<2>::type) == typeid(int));
}

TEST_CASE(function_traits_on_lambda)
{
    auto lambda = [](bool, char, int) { return 0; };
    TEST_CHECK(function_traits<decltype(lambda)>::arity == 3);
    TEST_CHECK(typeid(function_traits<decltype(lambda)>::function_type) == typeid(int(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(lambda)>::return_type) == typeid(int));
    TEST_CHECK(typeid(function_traits<decltype(lambda)>::stl_function_type) == typeid(std::function<int(bool, char, int)>));
    TEST_CHECK(typeid(function_traits<decltype(lambda)>::pointer) == typeid(int(*)(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(lambda)>::args<0>::type) == typeid(bool));
    TEST_CHECK(typeid(function_traits<decltype(lambda)>::args<1>::type) == typeid(char));
    TEST_CHECK(typeid(function_traits<decltype(lambda)>::args<2>::type) == typeid(int));
}

TEST_CASE(function_traits_on_stl_function)
{
    std::function<int(bool, char, int)> stl_function;
    TEST_CHECK(function_traits<decltype(stl_function)>::arity == 3);
    TEST_CHECK(typeid(function_traits<decltype(stl_function)>::function_type) == typeid(int(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(stl_function)>::return_type) == typeid(int));
    TEST_CHECK(typeid(function_traits<decltype(stl_function)>::stl_function_type) == typeid(std::function<int(bool, char, int)>));
    TEST_CHECK(typeid(function_traits<decltype(stl_function)>::pointer) == typeid(int(*)(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(stl_function)>::args<0>::type) == typeid(bool));
    TEST_CHECK(typeid(function_traits<decltype(stl_function)>::args<1>::type) == typeid(char));
    TEST_CHECK(typeid(function_traits<decltype(stl_function)>::args<2>::type) == typeid(int));
}

TEST_CASE(function_traits_on_member_function)
{
    TEST_CHECK(function_traits<decltype(&ClassType::member_function)>::arity == 3);
    TEST_CHECK(typeid(function_traits<decltype(&ClassType::member_function)>::function_type) == typeid(int(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(&ClassType::member_function)>::return_type) == typeid(int));
    TEST_CHECK(typeid(function_traits<decltype(&ClassType::member_function)>::stl_function_type) == typeid(std::function<int(bool, char, int)>));
    TEST_CHECK(typeid(function_traits<decltype(&ClassType::member_function)>::pointer) == typeid(int(*)(bool, char, int)));
    TEST_CHECK(typeid(function_traits<decltype(&ClassType::member_function)>::args<0>::type) == typeid(bool));
    TEST_CHECK(typeid(function_traits<decltype(&ClassType::member_function)>::args<1>::type) == typeid(char));
    TEST_CHECK(typeid(function_traits<decltype(&ClassType::member_function)>::args<2>::type) == typeid(int));
}

TEST_CASE(function_traits_on_function_object)
{
    TEST_CHECK(function_traits<ClassType>::arity == 3);
    TEST_CHECK(typeid(function_traits<ClassType>::function_type) == typeid(int(bool, char, int)));
    TEST_CHECK(typeid(function_traits<ClassType>::return_type) == typeid(int));
    TEST_CHECK(typeid(function_traits<ClassType>::stl_function_type) == typeid(std::function<int(bool, char, int)>));
    TEST_CHECK(typeid(function_traits<ClassType>::pointer) == typeid(int(*)(bool, char, int)));
    TEST_CHECK(typeid(function_traits<ClassType>::args<0>::type) == typeid(bool));
    TEST_CHECK(typeid(function_traits<ClassType>::args<1>::type) == typeid(char));
    TEST_CHECK(typeid(function_traits<ClassType>::args<2>::type) == typeid(int));
}
