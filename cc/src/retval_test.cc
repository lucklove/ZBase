#include "retval.hh"
#include <iostream>
#include <string>

using namespace zbase;

struct Test {
	int x;
	Test(Test&&v) : x(v.x) { std::cout << "move" << std::endl; }
	Test(const Test &v) : x(v.x) { std::cout << "copy" << std::endl; }
	Test(int v) : x(v) {}
	Test& operator=(const Test&) = default;
	Test() {}
};

auto
func()
{
	return makeRet(1, 2, 'a', 'b', "hello", "world", Test(3));
}

int
main(int argc, char *argv[])
{
	int x, y;
	char a, b;
	std::string f, g;
	Test t;
	RetVal(x, y, a, b, f, g, t) = func();
	std::cout << x << std::endl;
	std::cout << y << std::endl;
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << f << std::endl;
	std::cout << g << std::endl;
	return 0;
}
