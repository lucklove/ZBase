#include "bits.h"
#include <stdio.h>

int
main(int argc, char *argv[])
{
	int i = 0;
	bits_set(&i, 9);
	printf("%d\n", i);
	printf("%d\n", bits_test(&i, 9));
	printf("%d\n", i);
	bits_clr(&i, 9);
	printf("%d\n", i);
	printf("%d\n", bits_test(&i, 9));
	return 0;
}
