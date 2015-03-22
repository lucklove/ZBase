#pragma once

#include <tuple>
#include <type_traits>
#include <functional>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "exception.hh"

namespace zbase {

namespace matrix {

using boost::numeric::ublas::prod;

using slice_t = boost::numeric::ublas::slice;

using range_t = boost::numeric::ublas:: range;

template<typename M>
using matrix_row_t = boost::numeric::ublas::matrix_row<M>;

template<typename M>
using matrix_col_t = boost::numeric::ublas::matrix_column<M>;

template<typename T>
using vector_t = boost::numeric::ublas::vector<T>;

template<typename T>
using matrix_t = boost::numeric::ublas::matrix<T>;

struct MatrixError : public zbase::Exception {
	using Exception::Exception;
};

template<typename ItemType>
void
delRow(matrix_t<ItemType>& mat, size_t row_n)
{
	for(size_t i = row_n + 1; i < mat.size1(); ++i) {
		matrix_row_t<matrix_t<ItemType>> mr(mat, i);
		matrix_row_t<matrix_t<ItemType>> mpr(mat, i - 1);
		mpr = mr;
	}
	mat.resize(mat.size1() - 1, mat.size2());
}

template<typename ItemType>
void
delCol(matrix_t<ItemType>& mat, size_t col_n)
{
	for(size_t i = col_n + 1; i < mat.size2(); ++i) {
		matrix_col_t<matrix_t<ItemType>> mc(mat, i);
		matrix_col_t<matrix_t<ItemType>> mpc(mat, i - 1);
		mpc = mc;
	}
	mat.resize(mat.size1(), mat.size2() - 1);
}

/**
 * \note src dst 可以引用自相同对象
 */ 
template<typename ItemType>
void
mergeByRow(matrix_t<ItemType>& src, const matrix_t<ItemType>& dst)
{
	if(src.size1() != dst.size1())
		DEBUG_THROW(MatrixError, "src.size1() != dst.size1()");

	size_t old_size = src.size2();	
	src.resize(src.size1(), src.size2() + dst.size2());
	for(size_t i = 0; i < src.size1(); ++i) {
		for(size_t j = old_size; j < src.size2(); ++j)
			src(i, j) = dst(i, j - old_size);
	}
}

/**
 * \note src dst 可以引用自相同对象
 */ 
template<typename ItemType>
void
mergeByCol(matrix_t<ItemType>& src, const matrix_t<ItemType>& dst)
{
	if(src.size2() != dst.size2())
		DEBUG_THROW(MatrixError, "src.size2() != dst.size2()");

	size_t old_size = src.size1();	
	src.resize(src.size1() + dst.size1(), src.size2());
	for(size_t i = old_size; i < src.size1(); ++i) {
		for(size_t j = 0; j < src.size2(); ++j)
			src(i, j) = dst(i - old_size, j);
	}
}

template<typename ItemType>
ItemType
sum(const vector_t<ItemType>& vec)
{
	ItemType s = ItemType();
	for(ItemType i : vec)
		s += i;
	return s;
}

template<typename Container, typename ItemType = 
	typename std::remove_cv<
		typename std::remove_reference<
			decltype(*static_cast<Container *>(nullptr)->begin())
		>::type
	>::type
>
auto
mean(const Container& cont, const std::function<ItemType(ItemType)>&
	 pre_func = [](auto x) { return x; })
{
	ItemType sum = ItemType();
	for(ItemType it : cont)
		sum += pre_func(it);
	return sum / cont.size();
}

/**
 * \brief 求方差
 */
template<typename Container, typename ItemType = 
	typename std::remove_cv<
		typename std::remove_reference<
			decltype(*static_cast<Container *>(nullptr)->begin())
		>::type
	>::type
>
auto
var(const Container& cont)
{
	/** XXX: 有没有优雅点的用法呢? */
	return (mean(cont, std::function<ItemType(ItemType)>([](auto x) { return x * x; }))
			- mean(cont) * mean(cont)) * cont.size() / (cont.size() - 1);
}

std::tuple<matrix_t<double>, matrix_t<double>> eig(matrix_t<double> mat);
matrix_t<double> zscore(matrix_t<double> mat);
matrix_t<double> cov(matrix_t<double> mat);

}	/**< namespace matrix */

}	/**< namespace zbase */
