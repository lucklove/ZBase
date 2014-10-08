#include "compress.h"
#include <string.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	compressEncodeFile("./before_compress", "./after_compress");	
	compressDecodeFile("./after_compress", "./before_compress");	
	return 0;
}
