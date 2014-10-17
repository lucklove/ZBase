#include "mem.h"
#include <stdio.h>

struct MyStruct {
	int i;
	char c;
	char *cp;
};

int
main(int argc, char *argv[])
{
	struct MyStruct t1 = { 1, '1', "12" };
	struct MyStruct t2 = { 2, '2', "22" };
	mem_t mem = makeMem(1);
	SET_TYPE_MEM(&mem, struct MyStruct, 0, t1);
	SET_TYPE_MEM(&mem, struct MyStruct, 2, t2);
	printf("There are %d items\n", GET_ITEM_NUM(mem, struct MyStruct));
	printf("%d %c %s %p\n", 
		GET_TYPE_MEM(&mem, struct MyStruct, 0)->i,
		GET_TYPE_MEM(&mem, struct MyStruct, 0)->c,
		GET_TYPE_MEM(&mem, struct MyStruct, 0)->cp,
		GET_TYPE_MEM(&mem, struct MyStruct, 0));
	printf("%d %c %s %p\n", 
		GET_TYPE_MEM(&mem, struct MyStruct, 2)->i,
		GET_TYPE_MEM(&mem, struct MyStruct, 2)->c,
		GET_TYPE_MEM(&mem, struct MyStruct, 2)->cp,
		GET_TYPE_MEM(&mem, struct MyStruct, 2));
	mem_t mem1 = makeMem(1);
	memCpy(&mem1, getMemPtr(&mem, 0, 0), mem.index);
	printf("%d %c %s %p\n", 
		GET_TYPE_MEM(&mem1, struct MyStruct, 0)->i,
		GET_TYPE_MEM(&mem1, struct MyStruct, 0)->c,
		GET_TYPE_MEM(&mem1, struct MyStruct, 0)->cp,
		GET_TYPE_MEM(&mem1, struct MyStruct, 0));
	printf("%d %c %s %p\n", 
		GET_TYPE_MEM(&mem1, struct MyStruct, 2)->i,
		GET_TYPE_MEM(&mem1, struct MyStruct, 2)->c,
		GET_TYPE_MEM(&mem1, struct MyStruct, 2)->cp,
		GET_TYPE_MEM(&mem1, struct MyStruct, 2));
	memCat(&mem1, getMemPtr(&mem, 0, 0), mem.index);
	printf("%d %c %s %p\n", 
		GET_TYPE_MEM(&mem1, struct MyStruct, 3)->i,
		GET_TYPE_MEM(&mem1, struct MyStruct, 3)->c,
		GET_TYPE_MEM(&mem1, struct MyStruct, 3)->cp,
		GET_TYPE_MEM(&mem1, struct MyStruct, 3));
	setMemIndex(&mem1, 0);
	memSet(&mem1, 0, sizeof(struct MyStruct) * 7);
	printf("%d %c %s %p\n", 
		GET_TYPE_MEM(&mem1, struct MyStruct, 5)->i,
		GET_TYPE_MEM(&mem1, struct MyStruct, 5)->c,
		GET_TYPE_MEM(&mem1, struct MyStruct, 5)->cp,
		GET_TYPE_MEM(&mem1, struct MyStruct, 5));
	destroyMem(mem);
	destroyMem(mem1);
	return 0;
}
