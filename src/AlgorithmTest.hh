#pragma once

#include <ostream>

namespace zbase {

template<typename T>
class AlgorithmTest {
public:
	using size_t = unsigned int;
	AlgorithmTest(const T& val) : value(val) {}
	AlgorithmTest(const AlgorithmTest& ins) : value(ins.value) { ++assignment_count; }
	AlgorithmTest(AlgorithmTest&& ins) : value(ins.value) { ++ assignment_count; }
	AlgorithmTest& 
	operator=(const AlgorithmTest& item) 
	{ 
		value = item.value;
		++assignment_count;
		return *this;
	}
	bool operator<(const AlgorithmTest& param) { ++compare_count; return value < param.value; }	
	static const size_t getCompareCount() { return compare_count; }
	static const size_t getAssignmentCount() { return assignment_count; }
	friend std::ostream& 
	operator<<(std::ostream& os, const AlgorithmTest& item)
	{
		return os << item.value;
	}
private:
	T value;
	static size_t compare_count;
	static size_t assignment_count;
};

template<typename T>
typename AlgorithmTest<T>::size_t AlgorithmTest<T>::compare_count = 0;

template<typename T>
typename AlgorithmTest<T>::size_t AlgorithmTest<T>::assignment_count = 0;

}		/**< namespace zbase */
