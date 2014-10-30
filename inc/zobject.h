#pragma once

#include "mem.h"
#include "rb_tree.h"

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
int zRegistClass(const char *, const char *parent_name, void (*constructor)(struct ZObjInstance *, void *), 
void (*destructor)(struct ZObjInstance *), void *, unsigned int);
//int zRegistInterface(const char *interface_name, void *interface_body, unsigned int size_of_interface);
//void *zObjGetInterface(ZObjInstance *, const char *interfacename);

#define Z_OBJ_TO_CLASS(ins, class_name, type) ((type *)zGetClass(ins, class_name))
#define Z_OBJ_TO_INSTANCE(ins, class_name, type) ((type *)zGetInstance(ins, class_name))
