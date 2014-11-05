#include "zobject.h"
#include "mem.h"
#include "debug.h"
#include <stdio.h>

struct Cla1 {
	void (*func)(void);
};

struct Ins1 {
	const char *str;
};

static void
func1()
{
	printf("func1 is called\n");
}

static void
cons1(struct ZObjInstance *ins, void *data)
{
	printf("enter cons1\n");
	Z_MAKE_SPACE(ins, sizeof(struct Ins1));
	Z_OBJ_TO_INSTANCE(ins, NULL, struct Ins1)->str = "I am cla1\n";
	if(Z_HAVE_PARENT(ins)) {
		Z_MAKE_PARENT_SPACE(ins);
		Z_SET_CLASS(Z_PARENT_OBJ(ins), Z_PARENT_CLASS(ins));
		Z_CONS_PARENT(ins, data);
	}
}


static void
des1(struct ZObjInstance *ins)
{
	zDesInstanceSpace(ins);
	printf("enter des1\n");	
}

static void
test_init()
{
	struct Cla1 c1 = { func1 };
	zRegistClass("class1", NULL, cons1, des1, &c1, sizeof(c1));
}

int
main(int argc, char *argv[])
{
	memDebugInit();
	zObjInit();
	test_init();
        struct ZObjInstance * ins1 = zNewInstance("class1", NULL);
	Z_OBJ_TO_CLASS(ins1, NULL, struct Cla1)->func();
	printf(Z_OBJ_TO_INSTANCE(ins1, "class1", struct Ins1)->str);
	Z_DES_OBJ(ins1);
	memCheckLeak(stdout);
	memDebugRelease();
	return 0;
}
