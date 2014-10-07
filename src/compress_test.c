#include "compress.h"
#include <string.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	compressEncodeFile("./test.txt", "./test.txt.txt");	
	compressDecodeFile("./test.txt.txt", "./test.txt");	
	return 0;
}
