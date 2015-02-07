#pragma once

#include <iostream>
#include <sstream>

namespace zbase {

/**
 * \brief 根据参数类型使用 std::forward 传递对应参数。
 * 传递参数：按类型保持值类别(value catory) 和 const 修饰符。
 * 当表达式类型为函数或函数引用类型时，结果为左值(lvalue) ，否则：
 * 当且仅当左值引用类型时结果为左值（此时类型不变）；
 * 否则结果为对应的右值引用类型的消亡值(xvalue) 。
 */
#define zforward(_expr) std::forward<decltype(_expr)>(_expr)

/**
 * \brief 向文件流中输出数据
 * \param os 目标文件流
 * \param param 要输出的内容，可以是任意可输出类型
 */ 
template<typename _type>
inline void
fprint(std::ostream& os, const _type& param)
{
	os << param;
}

/**
 * \brief 向文件流输出数据
 * \param os 目标文件流
 * \param param1 第一个要输出的数据
 * \param params 除第一个外要输出的数据
 */ 
template<typename _type1, typename... _types>
inline void
fprint(std::ostream& os, const _type1& param1, const _types&... params)
{
	fprint(os, param1);
	fprint(os, params...);
}

/**
 * \brief 向文件中输出一行数据
 * \param os 目标文件流
 * \param params 要输出的各数据
 */ 
template<typename... _types>
inline void
fprintln(std::ostream& os, const _types&... params)
{
	fprint(os, params..., '\n');
}

/**
 * \brief 向标准输出输出数据
 */ 
template<typename... _types>
inline void
print(const _types&... params)
{
	fprint(std::cout, params...);
}

/**
 * \brief 向标准输出输出一行数据
 */ 
template<typename... _types>
inline void
println(const _types&... params)
{
	fprintln(std::cout, params...);
}	

template<typename T>
std::string
toString(T&& some_thing)
{
	std::string ret;
	std::stringstream s(ret);
	s << some_thing;
	s >> ret;
	return ret;
}	
	
template<typename Iter>
class Iterable {
public:
	Iterable(Iter begin_i, Iter end_i) : begin_iter(begin_i), end_iter(end_i) {}
	Iter begin() { return begin_iter; }
	Iter end() { return end_iter; }
private:
	Iter begin_iter;
	Iter end_iter;
};
	
template<typename Iter>
Iterable<Iter>
each(Iter&& begin, Iter&& end)
{
	return Iterable<Iter>(begin, end);
}

} 		/**< namespace zbase */
