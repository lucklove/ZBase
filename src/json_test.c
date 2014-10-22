#include "json.h"
#include "debug.h"
#include <stdio.h>

int
main(int argc, char *argv[])
{
	printf("---------------------------------\n");
	printf("%s\n", argv[1]);
	printf("---------------------------------\n");
	json_t json = makeJson(argv[1]);
	dumpJson(json);
	return 0;
}
