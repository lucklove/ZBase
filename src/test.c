#include "compress.h"
#include <string.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	char *data = "我去年卖了个表";
	compress_t session = makeCompressType(data, strlen(data));
	session = compressEncode(session);
	session = compressDecode(session);
	printf("%s\n", (char *)session.data);
	
//	destroyCompressType(session);
	return 0;
}
