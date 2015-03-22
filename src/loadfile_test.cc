#include "LoadFile.hh"
#include <iostream>

using namespace zbase;

static void func(char *str, int len) 
{
	for(int i = 0; i < len; ++i)
		std::cout << str[i];
	std::cout << std::endl;
}
int
main(int argc, char *argv[])
{
	LoadFile<char> file("load_file_test.txt");
	std::cout << "file size:" << file.size() << std::endl;
	for(int i = 0; i < file.size(); ++i)
		std::cout << file[i];
	std::cout << "--------------------------" << std::endl;
	func(file, file.size());
	return 0;
}
