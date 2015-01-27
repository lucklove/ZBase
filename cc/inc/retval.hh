/**************************************************************************
 * retval.hh								  *
 * 实现从同一函数返回多个值	                                          * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

namespace zbase {

/**
 * \brief 助手函数，将返对象中的各个值复制到接收对象的各个引用中
 * \param rec_tup 接收对象之tuple，存储真实接收者之引用
 * \param ret_tup 返回对象之tuple, 储存返回值副本集合
 * \note 为retval_t设计，不希望在其它地方被调用
 */
template<typename RecTuple, typename RetTuple, size_t... Is> 
void 
copy_retval_helper(RecTuple& rec_tup, RetTuple& ret_tup, 
			std::index_sequence<Is...>) 
{
	using swallow = int[];
	/** 编译时展开求值 */
	(void)swallow{0, (std::get<Is>(rec_tup) = std::get<Is>(ret_tup), 0)... };
}

/**
 * \brief 储存接收者的引用，并接收对应tuple的赋值
 */
template<typename... Types>	
class retval_t {
public:
	/**
 	 * \param receivers 接收者，必须为左值
 	 */
	retval_t(Types&... receivers) 
		: receiver(std::tuple<Types&...>(receivers...))
	{}

	/**
 	 * \param retval 返回值集合
 	 */ 
	void operator=(const std::tuple<Types...>& retval) {
		copy_retval_helper(receiver, retval, 
			std::make_index_sequence<sizeof...(Types)>());
	}
private:
	std::tuple<Types&...> receiver;		/**< 将接收者的引用储存于一个tuple */
};

/**
 * \brief 工具函数，便于用户床将retval_t对象
 * \param receivers 接收者，必须为左值
 * \return 接收者集合，可接收相应tuple赋值
 */ 
template<typename... Types>
retval_t<Types...>
RetVal(Types&... receivers)
{
	return retval_t<Types...>(receivers...);
}

/**
 * \brief 工具函数，便于用户创建返回值
 * \param retvals 返回值集合
 * \return 返回值组成的tuple
 */
template<typename... Types>
auto
makeRet(Types... retvals)
{
	return std::make_tuple(retvals...);
}

}			/**< namespace zbase */
