#pragma once

#include "mem.h"
#include "rb_tree.h"
#include <stdbool.h>
#include <stdlib.h>

struct ZObjClass;

struct ZObjInstance {
	struct ZObjClass *class;
	struct ZObjInstance *parent;
	void *instance_body;
};

struct ZObjClass {
	const char *class_name;
	struct ZObjClass *parent;
	void *(*constructor)(void *, void *);
	void (*destructor)(void *);
	unsigned int instance_size;
	mem_t interfaces;
	mem_t class_body;
	struct RBNode rb_node;
};

struct ZObjInterface {
	const char *interface_name;
	void *interface_body;
};

void zObjInit(void);
void *zGetClassByName(const char *, const char *);
void *zGetClassByInstance(struct ZObjInstance *, const char *);
void *zGetInstance(struct ZObjInstance *, const char *);
void *zGetInterfaceByName(const char *, const char *);
void *zGetInterfaceByInstance(struct ZObjInstance *, const char *);
void zAddInterface(const char *, const char *);
int zRegistInterface(const char *, const char *, unsigned int);
struct ZObjInstance *zNewInstance(const char *class_name, void *data);
void zDesInstance(struct ZObjInstance *);
int zRegistClass(const char *, const char *parent_name, void *(*constructor)(void *, void *), 
		void (*destructor)(void *), unsigned int, void *, unsigned int);

#define Z_OBJ_TO_CLASS(ins, name, type) ((type *)zGetClassByInstance(ins, name))
#define Z_OBJ_TO_INTERFACE(ins, interface_name, type) ((type *)zGetInterfaceByInstance(ins, interface_name))
#define Z_OBJ_TO_INSTANCE(ins, class_name, type) ((type *)zGetInstance(ins, class_name))
#define Z_IMP_INTERFACE(class_name, interface_name, type) ((type *)zGetInterfaceByName(class_name, interface_name))
#define Z_CLASS_TO_CLASS(self, name, type) ((type *)zGetClassByName(self, name))
#define Z_GET_CLASS(class_ptr, type) ((type *)getMemPtr(&class_ptr->class_body, 0, 0))
