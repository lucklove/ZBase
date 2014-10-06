#include "bits.h"
#include <stdio.h>

int
main(int argc, char *argv[])
{
	int i = 0;
	BITS_SET(&i, 9);
	printf("%d\n", i);
	printf("%d\n", BITS_TEST(&i, 9));
	printf("%d\n", i);
	BITS_CLR(&i, 9);
	printf("%d\n", i);
	printf("%d\n", BITS_TEST(&i, 9));
	return 0;
}
