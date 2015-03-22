#include "exception.hh"
#include <iostream>

using namespace zbase;

int
main(int argc, char *argv[])
{
	try {
		DEBUG_THROW(Exception, "hh", 0);
	} catch (Exception e) {
		std::cout << e << std::endl;
	}
	return 0;
}	
