#include "retval.hh"
#include <iostream>
#include <string>

using namespace zbase;

auto
func()
{
	return makeRet(1, 2, 'a', 'b', "hello", "world");
}

int
main(int argc, char *argv[])
{
	int x, y;
	char a, b;
	std::string f, g;
	RetVal(x, y, a, b, f, g) = func();
	std::cout << x << std::endl;
	std::cout << y << std::endl;
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << f << std::endl;
	std::cout << g << std::endl;
	return 0;
}
