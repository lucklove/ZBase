#include "utility.hh"

#include <iostream>
#include <array>
#include <vector>

using namespace zbase;

int
main(int argc, char *argv[])
{
	std::array<int, 3> a = {1,2};
	for(auto& i : each(&a[0], &a[3]))
		i = 3;
	for(auto& i : each(&a[0], &a[3]))
		std::cout << i << std::endl;
	return 0;
}
