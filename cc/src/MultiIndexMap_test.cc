#include "MultiIndexMap.hh"
#include <iostream>

using namespace zbase;

int
main(int argc, char *argv[])
{
	MultiIndexMap<double, int, int> m;
	m(1, 2) = 3.2;
	std::cout << m(1, 2) << std::endl;
}
