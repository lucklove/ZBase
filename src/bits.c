#include "bits.h"
#include <stdio.h>

void
bits_set(void *ptr, unsigned int index)
{
	BITS_SET(ptr, index);
}

void
bits_clr(void *ptr, unsigned int index)
{
	BITS_CLR(ptr, index);
}

int
bits_test(void *ptr, unsigned int index)
{
	return BITS_TEST(ptr, index);
}

void
bits_dump(void *ptr, unsigned int len)
{
	for(int i = 0; i < BITS_PER_BYTE * len; ++i) {
		printf("%d", bits_test(ptr, i));
		if((i + 1) % BITS_PER_BYTE == 0)
			printf(" ");
	}
	printf("\n");
}	
