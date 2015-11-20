#pragma once
#include <utility>

template <typename> struct AsyncWrapper;

/**
 * \brief [API] 包装异步调用, 通过链式调用避免回调地狱.
 * \param func 异步函数, 前n个参数是异步回调函数.
 * \example
 *      asyncWrap([](auto connect_callback)
 *      {
 *          async_connect("xxx.xxx.com", 8080, connect_callback);
 *      }).then([](auto send_callback, bool is_success)
 *      {
 *          if(is_success)
 *          {
 *              async_send("hello", send_callback);
 *          }
 *          else
 *          {
 *              log_fail();
 *          }
 *      }).then([](bool is_success)
 *      {
 *          if(is_success)
 *          {
 *              log_success();
 *          }
 *          else
 *          {
 *              log_fail();
 *          }
 *      }).apply();
 *      通过这种方式, 我们可以避免在回调函数connect_callback中调用async_send(这样需要嵌套send_callback).
 */
template <typename AsyncFuncT>
AsyncWrapper<AsyncFuncT> asyncWrap(AsyncFuncT func)
{
    return AsyncWrapper<AsyncFuncT>(func);   
}

template <typename AsyncFuncT>
struct AsyncWrapper 
{
public:
    AsyncWrapper(AsyncFuncT func) : async_func_{func}
    {
    }

    template <typename... FuncTs>
    auto then(FuncTs... callbacks)
    {
        return asyncWrap([=](auto... ps)
        {
            /** FIXME: internal compiler error in gcc */
            auto callback_wrapper = [=](auto callback)
            {   
                return [=](auto&&... params)
                { 
                    return callback(ps..., std::forward<decltype(params)>(params)...); 
                }; 
            };
            async_func_(callback_wrapper(callbacks)...);
        });
    }

    auto apply()
    {
        return async_func_();
    }
private:
    AsyncFuncT async_func_;
};
