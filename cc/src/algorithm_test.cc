#include "AlgorithmTest.hh"
#include "QuickSort.hh"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cmath>

using namespace zbase;

int
main(int argc, char *argv[])
{
	std::vector<AlgorithmTest<int>> v;
	int num;
	std::cout << "样本规模:";
	std::cin >> num;
	assert(num > 0);
	srand(time(nullptr));
	for(int i = 0; i < num; ++i)
		v.push_back(rand());
	zbase::quickSort(v.begin(), v.end());
	std::cout << "赋值次数:" << AlgorithmTest<int>::getAssignmentCount() << std::endl;
	std::cout << "比较次数:" << AlgorithmTest<int>::getCompareCount() << std::endl;
	std::cout << "赋值比值:" << AlgorithmTest<int>::getAssignmentCount() / (num * log(num) / log(2)) << std::endl;
	std::cout << "比较比值:" << AlgorithmTest<int>::getCompareCount() / (num * log(num) / log(2)) << std::endl;
	
	return 0;
}
