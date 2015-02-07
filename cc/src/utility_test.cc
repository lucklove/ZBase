#include "utility.hh"
#include "StringUtils.hh"

#include <iostream>
#include <array>
#include <vector>

using namespace zbase;

int
main(int argc, char *argv[])
{
	std::array<int, 3> a = {1, 2, 3};
	for(auto& i : each(&a[0], &a[3]))
		i = 3;
	for(auto& i : each(&a[0], &a[3]))
		std::cout << i << std::endl;
	std::cout << startsWith("xx", "xx_3322\nxx_3") << std::endl;
	std::cout << toString("1234") << std::endl;
	return 0;
}
