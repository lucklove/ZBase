Usefull utils
=============

[![Build Status](https://travis-ci.org/lucklove/ZBase.svg?branch=master)](https://travis-ci.org/lucklove/ZBase)
[![Coverage Status](https://coveralls.io/repos/lucklove/ZBase/badge.svg?branch=master&service=github)](https://coveralls.io/github/lucklove/ZBase?branch=master)

Design principle
----------------

- [Header only](https://github.com/lucklove/ZBase/tree/master/inc)
- Low dependence:  
    never dependent on other headers(include which from inc/) except which from ISO C++ and POSIX.

Introduction
------------

| Functionality | Documentation | Header    | Test  |
|---------------|---------------|-----------|-------|
| Unit testing  | [UnitTest.hh](#UnitTesthh) | UnitTest.hh | see [test](test) |
| Wrap async call to avoid callback hell | [AsyncWrapper.hh](#AsyncWrapperhh) | AsyncWrapper.hh | [here](test/AsyncWrapper.cc) |
| Danamic generic type | [Any.hh](#Anyhh) | Any.hh | [here](test/Any.cc) |
| Uninitialized concept | [Optional.hh](#Optionalhh) | Optional.hh | [here](test/Optional.cc) |
| multi-type, single value container | [Variant.hh](#Varianthh) | Variant.hh | [here](test/Variant.cc) | 

Usage
-----

To use the header, just copy which you need to your include dictionary.

UnitTest.hh
-----------

Extremely small unit testing framework.    
```c++
#define TEST_MAIN       /**< this will insert main function to this compile unit atomaticly */
#include "UnitTest.hh"

TEST_CASE(a_test_case)
{
    TEST_REQUIRE(condition);    /**< if this fail, the next lines in this case will not be executed */
    TEST_CHECK(condition);      /**< if this fail, failure will be report, but test go on */
    throw some_thing{msg};      /**< this exception will be caught by unit testing framework and a failure will be report */   
}

TEST_CASE(another_test_case)
{
    *(int *)0 = 0;              /**< the unit test framewrok will caught SIGSEGV and terminate all tests */
}
```

AsnycWrapper.hh
---------------

Designed to avoid deep callback.  
Compare codes bellow, you can't miss it.   
  
With lambda, we always do callbacks like this:
```c++
async_function(params, [](callback_param_decls)
{
    if(need_do_another_async_operation())
    {
        another_async_function(params, [](callback_param_decls)
        {
            if(need_do_yet_another_async_operation())
            {
                yet_another_async_function(params, [](callback_param_decls)
                {
                    /** what if we need more async operations? */
                }   
            }
        });
    }
});
```
Now, we can do it in this way:
```c++
asyncWrap([](auto callback, callback_param_decls)
{
    async_func(params, callback);
}).then([](auto callback, callback_param_decls)
{
    if(need_do_another_async_operation())
        another_async_function(params, callback);
}).then([](auto callback, callback_param_decls)
{
    if(need_do_yet_another_async_operation())
        yet_another_async_function(params, callback);
}) /**< if need more async operation, just write .then(...), and at then end of the then list, we should: */
.apply();
```

Any.hh
------

The Any class is a variant value type based on the second category.   
It supports copying of any value type and safe checked extraction of that value strictly against its type.   
```c++
Any a, b = 47;              /**< a contains null, b contains int(47) */
if(b.is<int>())             /**< check if b contains a int, should be true */
{
    a = b.cast<int>();      /**< take which b contains, now a and b both contain int(47) */
    Any c = a;              /**< c contains int instead of Any */
    std::cout << c.cast<bool>() << std::endl;   /**< this will throw std::bad_cast */
}
```

Optional.hh
-----------

Class template Optional is a wrapper for representing 'optional' (or 'nullable') objects who may not (yet) contain a valid value.
```c++
Optional<int> opt;      /**< maybe contains bool */
if(opt)                 /**< check if opt is initialized, will be false */
{
    /** do something */
}
else
{
    opt = 47;                           /**< now, opt is initialized */
    Optional<int> opt1 = opt;           /**< now, opt and opt1 contains the same thing */
    Optional<std::string> opt2 = opt;   /**< compile error */
    Optional<std::string> opt3 = 3;     /**< compile error */
}

```

Variant.hh
----------

    The variant class template is a safe, generic, stack-based discriminated union container,    
    Whereas standard containers such as std::vector may be thought of as "multi-value, single     
type," variant is "multi-type, single value."

```c++
Variant<int, bool, std::string> var;    /**< may contains int, bool or std::string */
var = 47;                               /**< now contains int, var.is<int>() will be true */
if(var.is<int>())                       /**< check if var contains int */
    std::cout << var.get<int>() << std::endl;                   /**< get int from variant */
Variant<int, bool, std::string> var1 = var;                     /**< var1 contains the same thing as var */
Variant<int, bool, std::string> var2 = "string";                /**< compile error */
Variant<int, bool, const char*> var3 = "const char*";           /**< ok */
Variant<int, bool, std::string> var4 = std::string{"string"};   /**< ok */
```
