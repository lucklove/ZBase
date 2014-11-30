#include "ref.h"
#include <stdio.h>

struct MyStruct {
	int val;
	ref_t ref;
};

static void
rel(ref_t *ref)
{
	printf("释放%p\n", ref);
}		

int
main(int argc, char *argv[])
{
	struct MyStruct mynode;
	mynode.ref = makeRef(rel);
	refGet(&mynode.ref);
	refPut(&mynode.ref);
	refPut(&mynode.ref);
	return 0;
}
