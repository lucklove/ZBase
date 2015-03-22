#include "random.h"
#include <stdio.h>
#include <time.h>

#define TEST_TIMES 1000000

int odd_num = 0;
int even_num = 0;

int
main(int argc, char *argv[])
{
	randSeed(time(NULL));
	for(int i = 0; i < TEST_TIMES; i++) {
		if(randGen() % 2 == 0) {
			even_num++;
		} else {
			odd_num++;
		}
	}
	printf("odd:%d\teven:%d\n", odd_num, even_num);
	return 0;
}
