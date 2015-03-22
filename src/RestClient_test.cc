#include "RestClient.hh"
#include <iostream>

using namespace zbase;

int
main(int argc, char *argv[])
{
	auto&& r = RestClient::post("http://115.28.32.115/cgi-bin/login.cgi",
		"text/json", "{\"username\":\"test\", \"password\":\"0060ec9a4efe96b911a5c4bc0452775713815ab9\", \"timestamp\":\"2000000000\"}");
	std::cout << r.body << std::endl;
	std::cout << r.code << std::endl;
}
