#include <iostream>
#include "http.hh"

using namespace zbase;
using namespace std;

int
main(int argc, char *argv[])
{
	Http http;
	auto&& r = http.syncPost("ddt888.sinaapp.com", "op=get&dest=user&username=test");
	if(r) {
		string s = r;
		cout << s << endl;
	} else {
		cout << "error: " << r.getCode() << " " << r.getError() << endl;
	}
}
