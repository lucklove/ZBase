#include "base64.h"
#include <stdio.h>
#include <string.h>
#include "bits.h"
#include "debug.h"
#include "compress.h"

int
main(int argc, char *argv[])
{
	base64EncodeFile("test.jpg", "test.txt");
//	compressEncodeFile("b64_before_compress", "after_compress");
//	compressDecodeFile("after_compress", "b64_before_compress");
//	base64DecodeFile("b64_before_compress", "before_compress");
        return 0;
}
