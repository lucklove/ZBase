#include <iostream>
#include "concurrency.hh"

using namespace zbase;

int
main(int argc, char *argv[])
{
	thread_pool tp{2, 
		    []() { std::cout << "thread begin" << std::endl; }, 
		    []() { std::cout << "thread end" << std::endl; }};
	tp.enqueue([] {
		std::cout << "hello" << std::endl;
	});
	return 0;
}
