#pragma once

#include <functional>

namespace zbase {

template<typename Iter>
void
quickSort(Iter begin, Iter end)
{
	auto bi = begin;
	auto ei = end - 1;
	if(bi > ei)
		return;
	auto k_index = bi;
	auto key = *k_index;
	while(bi < ei) {
		while(!(*ei < key) && ei > k_index) --ei;
		*k_index = *ei;
		k_index = ei;
		while(!(key < *bi) && bi < k_index) ++bi;
		*k_index = *bi;
		k_index = bi;
	}
	*k_index = key;
	quickSort(begin, k_index);
	quickSort(k_index + 1, end);
}

}               /**< namespace zbase */
