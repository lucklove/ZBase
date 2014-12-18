#include "parse.hh"
#include "StringTokenizer.hh"
#include <iostream>
#include <string>

using namespace zbase;

int
main(int argc, char *argv[])
{
	StringTokenizer st;
	std::string str{argv[1]};
	st = { str, '-' };
	if(!st.hasMoreTokens())
		return 0;
	while(st.hasMoreTokens()) {
		try {
			double var = parse<double>(st.nextToken());
			std::cout << "var = " << var <<std::endl;
		} catch(std::invalid_argument e) {
			std::cout << "invalid argument" <<std::endl;
		}
	}
	return 0;
}
