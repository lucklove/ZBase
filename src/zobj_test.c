#include "zobject.h"
#include "mem.h"
#include "debug.h"
#include <stdio.h>

struct MyClass {
	void (*hello)(void);
};

struct MyInstance {
	int val;
};

static void
haha()
{
	printf("hello\n");
}

static void
wuwu()
{
	printf("wuwu\n");
}

static void
constructor(struct ZObjInstance *ins, void *data)
{
	printf("cons...\n");
	ins->instance_body = makeMem(sizeof(struct MyInstance));
	GET_TYPE_MEM(&ins->instance_body, struct MyInstance, 0)->val = 3;
	if(ins->class->parent != NULL) {
		ins->parent = makeMem(sizeof(struct ZObjInstance));
		GET_TYPE_MEM(&ins->parent, struct ZObjInstance, 0)->class = ins->class->parent;
		ins->class->parent->constructor(getMemPtr(&ins->parent, 0, 0), data);
	}
}


static void
destructor(struct ZObjInstance *ins)
{
	printf("des...\n");
}

static void
test_init()
{
	struct MyClass first = { haha };
	struct MyClass sec = { wuwu };
	zRegistClass("1", NULL, constructor, destructor, &first, sizeof(first));
	zRegistClass("2", "1", constructor, destructor, &sec, sizeof(sec));
	zRegistClass("3", "2", constructor, destructor, &sec, sizeof(sec));
	zRegistClass("4", "3", constructor, destructor, &sec, sizeof(sec));
	zRegistClass("5", "4", constructor, destructor, &sec, sizeof(sec));
}

int
main(int argc, char *argv[])
{
	zObjInit();
	test_init();
        struct ZObjInstance * ins1 = zNewInstance("1", NULL);
	struct ZObjInstance * ins5 = zNewInstance("5", NULL);
	Z_OBJ_TO_CLASS(ins1, NULL, struct MyClass)->hello();
	printf("val = %d\n", Z_OBJ_TO_INSTANCE(ins1, "1", struct MyInstance)->val);
	Z_OBJ_TO_CLASS(ins5, "1", struct MyClass)->hello();
	printf("val = %d\n", Z_OBJ_TO_INSTANCE(ins5, "1", struct MyInstance)->val);
	return 0;
}
