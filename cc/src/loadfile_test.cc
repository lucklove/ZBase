#include "loadfile.hh"
#include <iostream>

using namespace zbase;

int
main(int argc, char *argv[])
{
	LoadFile<char> file("load_file_test.txt");
	char *data = file.data();
	std::cout << "file size:" << file.len() << std::endl;
	for(int i = 0; i < file.len(); ++i)
		std::cout << data[i];
	return 0;
}
