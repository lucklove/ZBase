#include <iostream>
#include "concurrency.hh"

using namespace zbase;

int
main(int argc, char *argv[])
{
	thread_pool{2, 
		    []() { std::cout << "帝球大傻逼" << std::endl; }, 
		    []() { std::cout << "thread end" << std::endl; }};
	
	task_pool{2, 
		    []() { std::cout << "帝球大傻逼" << std::endl; }, 
		    []() { std::cout << "thread end" << std::endl; }};
	return 0;
}
