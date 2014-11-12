#include "zobject.h"
#include "mem.h"
#include "debug.h"
#include <stdio.h>
#include <assert.h>

struct Cla1 {
	void (*func)(void);
};

struct Ins1 {
	const char *str;
};

struct Int1 {
	int (*fuvc)(void);
};

static void
func1()
{
	printf("func1 is called\n");
}

static int
fvck1()
{
	printf("fuvc1\n");
	return 0;
}

static int
fvck2()
{
	printf("fuvc2\n");
	return 0;
}

static void
func2()
{
	printf("func2 is called\n");
}

static void
cons1(struct ZObjInstance *ins, void *data)
{
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
}

static void
test_init()
{
	struct Cla1 c1 = { func1 };
	zRegistClass("class1", NULL, cons1, des1, &c1, sizeof(c1));
	zRegistClass("class2", "class1", cons1, des1, &c1, sizeof(c1));
	zRegistInterface("int1", NULL, sizeof(struct Int1));
	zAddInterface("class2", "int1");
	Z_IMP_INTERFACE("class2", "int1", struct Int1)->fuvc = fvck1;
	zRegistClass("class3", "class2", cons1, des1, &c1, sizeof(c1));
	Z_IMP_INTERFACE("class3", "int1", struct Int1)->fuvc = fvck2;
	zRegistClass("class4", "class3", cons1, des1, &c1, sizeof(c1));
	zRegistClass("class5", "class4", cons1, des1, &c1, sizeof(c1));
	Z_CLASS_TO_CLASS("class5", "class2", struct Cla1)->func = func2;
}

int
main(int argc, char *argv[])
{
	memDebugInit();
	zObjInit();
	test_init();
        struct ZObjInstance *ins1 = zNewInstance("class1", NULL);
	struct ZObjInstance *ins2 = zNewInstance("class2", NULL);
	struct ZObjInstance *ins3 = zNewInstance("class3", NULL);
	struct ZObjInstance *ins5 = zNewInstance("class5", NULL);
	Z_OBJ_TO_CLASS(ins1, NULL, struct Cla1)->func();
	printf(Z_OBJ_TO_INSTANCE(ins1, "class1", struct Ins1)->str);
	Z_OBJ_TO_CLASS(ins5, "class2", struct Cla1)->func();
	Z_OBJ_TO_CLASS(ins3, "class2", struct Cla1)->func();
	printf(Z_OBJ_TO_INSTANCE(ins5, "class1", struct Ins1)->str);
	Z_OBJ_TO_INTERFACE(ins5, "int1", struct Int1)->fuvc();
	Z_OBJ_TO_INTERFACE(ins2, "int1", struct Int1)->fuvc();
	Z_DES_OBJ(ins2);
	Z_DES_OBJ(ins1);
	Z_DES_OBJ(ins3);
	Z_DES_OBJ(ins5);
	memCheckLeak(stdout);
	memDebugRelease();
	return 0;
}
