#include "zobject.h"
#include "mem.h"
#include <stdio.h>
#include <assert.h>

struct anamal_class {
	void (*who_are_you)(struct ZObjInstance *);
};

struct fish_class {
	void (*swim)(struct ZObjInstance *);
};

struct dog_class {
	void (*run)(struct ZObjInstance *);
};

struct anamal_instance {
	const char *name;
};

struct fish_instance {
	int size;
};

struct dog_instance {
	float speed;
};

static void
anamal_tell(struct ZObjInstance *ins)
{
	const char *name = Z_OBJ_TO_INSTANCE(ins, "anamal", struct anamal_instance)->name;
	printf("I am an anamal, and my name is %s\n", name);
}

static void
fish_tell(struct ZObjInstance *ins)
{
	const char *name = Z_OBJ_TO_INSTANCE(ins, "anamal", struct anamal_instance)->name;
	printf("I am a fish, and my name is %s\n", name);
}

static void
dog_tell(struct ZObjInstance *ins)
{
	const char *name = Z_OBJ_TO_INSTANCE(ins, "anamal", struct anamal_instance)->name;
	printf("I am an dog, and my name is %s\n", name);
}

static void
fish_swim(struct ZObjInstance *ins)
{
	Z_OBJ_TO_CLASS(ins, "anamal", struct anamal_class)->who_are_you(ins);
	printf("I am %d and I can swim\n", Z_OBJ_TO_INSTANCE(ins, "fish", struct fish_instance)->size);
}

static void
dog_run(struct ZObjInstance *ins)
{
	Z_OBJ_TO_CLASS(ins, "anamal", struct anamal_class)->who_are_you(ins);
	printf("My speed is %f\n", Z_OBJ_TO_INSTANCE(ins, "dog", struct dog_instance)->speed);
}

static void *
anamal_cons(void *ins, void *data)
{
	struct anamal_instance *instance = ins;
	instance->name = data;
	return NULL;
}
	
static void *
fish_cons(void *ins, void *data)
{
	struct fish_instance *instance = ins;
	instance->size = 2;
	return data;
}

static void
anamal_des(void *body)
{
	printf("anamal des\n");
}

static void
fish_des(void *body)
{
	printf("fish des\n");
}

static void *
dog_cons(void *ins, void *data)
{
	struct dog_instance *instance = ins;
	instance->speed = 3.5;
	return data;
}

static void
dog_des(void *body)
{
	printf("dog des\n");
}

static void
test_init()
{
	struct anamal_class anamal_struct = { anamal_tell };
	zRegistClass("anamal", NULL, anamal_cons, anamal_des, 
		sizeof(struct anamal_instance), &anamal_struct, sizeof(anamal_struct));
	struct fish_class fish_struct = { fish_swim };
	zRegistClass("fish", "anamal", fish_cons, fish_des,
		sizeof(struct fish_instance), &fish_struct, sizeof(fish_struct));
	Z_CLASS_TO_CLASS("fish", "anamal", struct anamal_class)->who_are_you = fish_tell;
	struct dog_class dog_struct = { dog_run };
	zRegistClass("dog", "anamal", dog_cons, dog_des,
		sizeof(struct dog_instance), &dog_struct, sizeof(dog_struct));
	Z_CLASS_TO_CLASS("dog", "anamal", struct anamal_class)->who_are_you = dog_tell;
}

static void
tell(struct ZObjInstance *ins)
{
	Z_OBJ_TO_CLASS(ins, "anamal", struct anamal_class)->who_are_you(ins);
}

int
main(int argc, char *argv[])
{
	zObjInit();
	test_init();
	struct ZObjInstance *ins = zNewInstance("anamal", "aiai");
	tell(ins);	
	zDesInstance(ins);	
	ins = zNewInstance("fish", "yuyu");
	tell(ins);
	Z_OBJ_TO_CLASS(ins, NULL, struct fish_class)->swim(ins);
	zDesInstance(ins);	
	return 0;
}
