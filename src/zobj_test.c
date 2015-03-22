/**************************************************************************
 * zobj_test.c                                                            * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#include "zobject.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * To use the object system, we should define a struct which contains
 * some function pointers, and thes function pointer point to the
 * functions we defined. By this way, we have defined our "class methods".
 */
struct anamal_class {
	void (*who_are_you)(struct ZObjInstance *);
};

struct fish_class {
	void (*swim)(struct ZObjInstance *);
};

struct dog_class {
	void (*run)(struct ZObjInstance *);
};

/**
 * The interface is the same with the class, you shoud define a struct to
 * contain function pointer.
 */
struct eat_interface {
	void (*eat)(void);
};

struct slow_eat_interface {
	void (*slow_eat)(void);
};

/**
 * To get our instance, we should define a struct which contains some variables,
 * and we should regard these variables is in our "class".
 */
struct anamal_instance {
	char *name;
};

struct fish_instance {
	int size;
};

struct dog_instance {
	float speed;
};

/** functions in our class. */
static void
anamal_tell(struct ZObjInstance *ins)
{
	const char *name = Z_OBJ_TO_INSTANCE(ins, "anamal", struct anamal_instance)->name;
	printf("I am an anamal, and my name is %s\n", name);
}

/** function in interface. */
static void
eat()
{
	printf("I am eating...\n");
}

static void
slow_eat()
{
	printf("I am eat slowly...\n");
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
	printf("I am sized %d and I can swim\n", Z_OBJ_TO_INSTANCE(ins, "fish", struct fish_instance)->size);
}

static void
dog_run(struct ZObjInstance *ins)
{
	Z_OBJ_TO_CLASS(ins, "anamal", struct anamal_class)->who_are_you(ins);
	printf("My speed is %f\n", Z_OBJ_TO_INSTANCE(ins, "dog", struct dog_instance)->speed);
}

/**  To register a class, we should provide a constructor. */
static void *
anamal_cons(void *ins, void *data)
{
	struct anamal_instance *instance = ins;
	instance->name = malloc(strlen(data));
	assert(instance->name != NULL);
	strcpy(instance->name, data);
	printf("construct anamal %s\n", (char *)data);
	return NULL;
}
	
static void *
fish_cons(void *ins, void *data)
{
	struct fish_instance *instance = ins;
	instance->size = 2;
	char *p = malloc(strlen(data) + 1);
	strcpy(p, data);
	return p;
}

/** and we should provide a destructor. */
static void
anamal_des(void *body)
{
	printf("destruct anamal %s\n", ((struct anamal_instance *)body)->name);
	free(((struct anamal_instance *)body)->name);
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
	char *p = malloc(strlen(data) + 1);
	strcpy(p, data);
	return p;
}

static void
dog_des(void *body)
{
	printf("dog des\n");
}

static void
test_init()
{
	/** 
 	 * to register an interface, we should provide a 
 	 * interface name and a size of interface struct. 
 	 */
	zRegistInterface("eat", sizeof(struct eat_interface));
	zRegistInterface("slow_eat", sizeof(struct slow_eat_interface));
	/**
  	 * add a parent for interface slow_eat, and the 
  	 * parent must be registed, here we chose eat,
  	 * we can add as many parents as we want.
  	 */
	zInterfaceAddParent("slow_eat", "eat");
	/** define a anamal_class struct so we can register it. */
	struct anamal_class anamal_struct = { anamal_tell };
	zRegistClass("anamal", NULL, anamal_cons, anamal_des, 
		sizeof(struct anamal_instance), &anamal_struct, sizeof(anamal_struct));
	/** add interface for class "anamal" */
	zAddInterface("anamal", "eat");
	/** 
 	 * by assign a function pointer to the struct member, 
 	 * we implement a method in interface.
 	 */
	Z_IMP_INTERFACE("anamal", "eat", struct eat_interface)->eat = eat;
	struct fish_class fish_struct = { fish_swim };
	zRegistClass("fish", "anamal", fish_cons, fish_des,
		sizeof(struct fish_instance), &fish_struct, sizeof(fish_struct));
	zAddInterface("fish", "slow_eat");
	Z_IMP_INTERFACE("fish", "eat", struct eat_interface)->eat = eat;
	Z_IMP_INTERFACE("fish", "slow_eat", struct slow_eat_interface)->slow_eat = slow_eat;
	/** by this way, we override the old method in class "anamal". */
	Z_CLASS_TO_CLASS("fish", "anamal", struct anamal_class)->who_are_you = fish_tell;
	struct dog_class dog_struct = { dog_run };
	zRegistClass("dog", "anamal", dog_cons, dog_des,
		sizeof(struct dog_instance), &dog_struct, sizeof(dog_struct));
	Z_CLASS_TO_CLASS("dog", "anamal", struct anamal_class)->who_are_you = dog_tell;
}

/** 
 * call the who_are_you func in class anamal, before the 
 * call, convert sub class to father class "anamal".
 */
static void
tell(struct ZObjInstance *ins)
{
	Z_OBJ_TO_CLASS(ins, "anamal", struct anamal_class)->who_are_you(ins);
}

int
main(int argc, char *argv[])
{
	zObjInit(NULL);
	test_init();			/**< register class and interface. */
	struct ZObjInstance *ins = zNewInstance("anamal", "aiai");
	tell(ins);
	/** interface test. */	
	Z_OBJ_TO_INTERFACE(ins, "eat", struct eat_interface)->eat();
	zDesInstance(ins);	
	ins = zNewInstance("fish", "yuyu");
	tell(ins);
	/** class test. */
	Z_OBJ_TO_CLASS(ins, NULL, struct fish_class)->swim(ins);
	Z_OBJ_TO_INTERFACE(ins, "eat", struct eat_interface)->eat();
	/** interface inheritance test. */
	Z_OBJ_TO_INTERFACE(ins, "slow_eat", struct slow_eat_interface)->slow_eat();
	/** refrence test */
	zObjDecRef(ins);
	/** 
 	 * try block test, if success, the three instance
 	 * "haha", "xixi", "kaka" should be release automatically.
 	 * (and don't ask why use such fool names.)
 	 */
	try {
		zNewInstance("anamal", "haha");
		zNewInstance("anamal", "xixi");
		zNewInstance("anamal", "kaka");
	} catch(ins, "base") {
		/** 
 		 * it should never rush to here, because we have 
 		 * no class named "base", just to avoid warning.
 		 */
		zDesInstance(ins);
	} finally {		/**< catch and finally is must. ╭(╯^╰)╮ */
		/**< do nothing. */
	}
	return 0;
}
