#include "opt.h"
#include <stdlib.h>
#include <stdio.h>

void opt_noreturn opt_pure my_exit(void);

void opt_construct(101) befor()
{
	printf("hello ");
}

int
return_const()
{
	return 3;
}

void opt_destruct(101) after()
{
	printf("world\n");
}


int
main(int argc, char *argv[])
{
}

void 
my_exit()
{
	exit(0);
}
