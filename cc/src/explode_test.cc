#include "explode.hh"
#include <iostream>

using namespace zbase;

int
main(int argc, char *argv[])
{
	std::string str{"the quick brown fox over the lazy dog"};
	std::vector<std::string> v = explode(str, ' ');
	for(auto n : v)
		std::cout << n << std::endl;

	return 0;
}
