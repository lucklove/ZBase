#include "utility.hh"

#include <iostream>
#include <fstream>
using namespace zbase;

int
main(int argc, char *argv[])
{
	println("this", ' ', "line", ' ', 'w', "ill", " ", "be out put at once");
	print("this is not ", 1, "line", 3.24, 34);
	std::ofstream file{"utility_test.txt"};
	fprintln(file, "this is a test file created by utility_test");
	return 0;
}
