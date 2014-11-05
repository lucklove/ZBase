#pragma once

#include "mem.h"
#include "rb_tree.h"
#include <stdbool.h>
#include <stdlib.h>

struct ZObjClass;

struct ZObjInstance {
	struct ZObjClass *class;
	mem_t parent;
	mem_t instance_body;
};

struct ZObjClass {
	const char *class_name;
	struct ZObjClass *parent;
	void (*constructor)(struct ZObjInstance *, void *);
	void (*destructor)(struct ZObjInstance *);
//	mem_t interfaces;
	mem_t class_body;
	struct RBNode rb_node;
};

/*
struct ZObjInterface {
	mem_t interface_name;
	mem_t parent;
	mem_t interface_body;
};
*/

void dump_class_tree(void);
void zObjInit(void);
void dump_class_list(void);
void *zGetClass(struct ZObjInstance *, const char *);
void *zGetInstance(struct ZObjInstance *, const char *);
struct ZObjInstance *zNewInstance(const char *class_name, void *data);
void zDesInstance(struct ZObjInstance *);
int zRegistClass(const char *, const char *parent_name, void (*constructor)(struct ZObjInstance *, void *), 
		void (*destructor)(struct ZObjInstance *), void *, unsigned int);
void zDesInstanceSpace(struct ZObjInstance *);
//int zRegistInterface(const char *interface_name, void *interface_body, unsigned int size_of_interface);
//void *zObjGetInterface(ZObjInstance *, const char *interfacename);

#define Z_OBJ_TO_CLASS(ins, class_name, type) ((type *)zGetClass(ins, class_name))
#define Z_OBJ_TO_INSTANCE(ins, class_name, type) ((type *)zGetInstance(ins, class_name))
#define Z_HAVE_PARENT(ins) (ins->class->parent)
#define Z_PARENT_OBJ(ins) (GET_TYPE_MEM(&ins->parent, struct ZObjInstance, 0))
#define Z_PARENT_CLASS(ins) (ins->class->parent)
#define Z_MAKE_PARENT_SPACE(ins) (ins->parent = makeMem(sizeof(struct ZObjInstance)))
#define Z_MAKE_SPACE(ins, size) (ins->instance_body = makeMem(size))
#define Z_SET_CLASS(ins, _class) (ins->class = _class)
#define Z_CONS_PARENT(ins, data) (Z_PARENT_CLASS(ins)->constructor(getMemPtr(&ins->parent, 0, 0), data))
#define Z_DES_OBJ(ins) 			\
do {					\
	zDesInstance(ins);		\
	free(ins);			\
} while(0)
