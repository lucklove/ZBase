#include "zobject.h"
#include "mem.h"
#include "rb_tree.h"
#include "container.h"
#include "opt.h"
#include "bits.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define to_class(node) container_of(node, struct ZObjClass, rb_node)
#define to_interface(node) container_of(node, struct InterfaceInfo, rb_node)

struct InterfaceInfo {
	const char *interface_name;
	struct InterfaceInfo *parent;
	unsigned int struct_size;
	struct RBNode rb_node;
};
	
// 保存类型
static RBTree class_tree;
static RBTree interface_tree;

/*
static void
dump_class_node(struct RBNode *tree)
{
	printf("class : %s\n", to_class(tree)->class_name);
	if(tree->rb_left != NULL)
		dump_class_node(tree->rb_left);
	if(tree->rb_right != NULL)
		dump_class_node(tree->rb_right);
}

void
dump_class_tree()
{
	if(rbGetRoot(class_tree) != NULL)
		dump_class_node(rbGetRoot(class_tree));
}
*/

//need by makeRBTree()
static void *
get_class(struct RBNode *node)
{
	return to_class(node);
}

static void *
get_interface(struct RBNode *node)
{
	return to_interface(node);
}
	
//need by makeRBTree()
static int
cmp_class(void *n1, void *n2)
{
	return strcmp(((struct ZObjClass *)n1)->class_name, 
		((struct ZObjClass *)n2)->class_name);
}

static int
cmp_interface(void *n1, void *n2)
{
	return strcmp(((struct InterfaceInfo *)n1)->interface_name, 
		((struct InterfaceInfo *)n2)->interface_name);
}

//need by makeRBTree()
static struct RBNode *
make_class(void *class_info)
{
	struct ZObjClass *class = malloc(sizeof(struct ZObjClass));
	assert(class != NULL);
	*class = *(struct ZObjClass *)class_info;
	return &class->rb_node;
}

static struct RBNode *
make_interface(void *interface_info)
{
	struct InterfaceInfo *info = malloc(sizeof(struct InterfaceInfo));
	assert(info != NULL);
	*info = *(struct InterfaceInfo *)interface_info;
	return &info->rb_node;
}

//need by makeRBTree()
static void
free_class(struct RBNode *node)
{
	free(to_class(node));
}

static void
free_interface(struct RBNode *node)
{
	free(to_interface(node));
}

//init object system
void
zObjInit()
{
	class_tree = makeRBTree(get_class, cmp_class, make_class, free_class);
	interface_tree = makeRBTree(get_interface, cmp_interface, make_interface, free_interface);
}

//find class in rb_tree by class name
static struct ZObjClass *
find_class(const char *name)
{
	struct ZObjClass tmp_class = { name };
	struct RBNode *node = rbSearch(class_tree, &tmp_class);
	if(node == NULL)
		return NULL;
	return to_class(node);
}

static struct InterfaceInfo *
find_interface(const char *name)
{
	struct InterfaceInfo tmp_info = { name };
	struct RBNode *node = rbSearch(interface_tree, &tmp_info);
	if(node == NULL)
		return NULL;
	return to_interface(node);
}

int	
zRegistClass(const char *class_name, const char *parent_name, void (*cons)(struct ZObjInstance *, void *),
	void (*des)(struct ZObjInstance *), void *class_body, unsigned int size_of_class)
{
	mem_t class = makeMem(size_of_class);
	struct ZObjClass *parent = NULL;
	if(find_class(class_name) != NULL) {
		printf("CRITICAL: redefined class %s.\n", class_name);
		return -1;
	}
	if(parent_name != NULL) {
		parent = find_class(parent_name);
		if(parent == NULL)
			printf("CRITICAL: parent not found.\n");
	}
	if(class_body != NULL)
		memCpy(&class, class_body, size_of_class);
	struct ZObjClass  reg_class = { class_name, parent, cons, des, makeMem(0), class };
	rbInsert(&class_tree, &reg_class);
	return 0;
}

int
zRegistInterface(const char *interface_name, const char *parent_name, unsigned int size_of_interface)
{
	struct InterfaceInfo *parent = find_interface(parent_name);
	if(parent == NULL)
		printf("CRITICAL: interface %s not found.\n", parent_name);
	struct InterfaceInfo reg_interface = { interface_name, parent, size_of_interface };
	rbInsert(&interface_tree, &reg_interface);
	return 0;
}

static void *
get_interface_in_class(const char *class_name, const char *interface_name)
{
	struct ZObjClass *dst_class = find_class(class_name);
	if(dst_class == NULL) {
		printf("CRITICAL: class %s not found.\n", class_name);
		return NULL;
	}
	for(int i = 0; i < GET_ITEM_NUM(dst_class->interfaces, struct ZObjInterface); ++i) {
		if(strcmp(interface_name, GET_TYPE_MEM(&dst_class->interfaces,
			struct ZObjInterface, i)->interface_name) == 0) 
			return GET_TYPE_MEM(&dst_class->interfaces, struct ZObjInterface, i)->interface_body;
	}
	if(dst_class->parent != NULL) {
		return get_interface_in_class(dst_class->parent->class_name, interface_name);
	}
	return NULL;
}

void
zAddInterface(const char *class_name, const char *interface_name)
{
	struct ZObjClass *dst_class = find_class(class_name);
	struct InterfaceInfo *dst_interface = find_interface(interface_name);
	if(dst_class == NULL) {
		printf("CRITICAL: class %s not found.\n", class_name);
		return;
	}
	if(dst_interface == NULL) {
		printf("CRITICAL: interface %s not found.\n", interface_name);
		return;
	}
	void *interface_body = malloc(dst_interface->struct_size);
	assert(interface_body != NULL);
	memset(interface_body, 0, dst_interface->struct_size);
	ADD_ITEM(&dst_class->interfaces, struct ZObjInterface, 
		((struct ZObjInterface){ interface_name, interface_body }));
}

	
void *
zGetInterfaceByName(const char *class_name, const char *interface_name)
{
	void *ret = get_interface_in_class(class_name, interface_name);
	if(ret == NULL)	
		printf("CRITICAL: interface %s not found.\n", interface_name);
	return ret;
}

void *
zGetInterfaceByInstance(struct ZObjInstance *ins, const char *interface_name)
{
	return zGetInterfaceByName(ins->class->class_name, interface_name);
}

struct ZObjInstance *
zNewInstance(const char *class_name, void *data)
{
	struct ZObjClass *dst_class = find_class(class_name);
	if(opt_unlikely(dst_class == NULL)) {
		printf("CRITICAL: class %s not found.\n", class_name);
		return NULL;
	}
	struct ZObjInstance *ins = malloc(sizeof(struct ZObjInstance));
	assert(ins != NULL && "malloc failed!");	
	ins->class = dst_class;
	dst_class->constructor(ins, data);
	if(dst_class->parent != NULL) {
		if(getMemIndex(ins->parent) == 0) {
			ins->parent = makeMem(sizeof(struct ZObjInstance));
			ADD_ITEM(&ins->parent, struct ZObjInstance, 
				*zNewInstance(dst_class->parent->class_name, NULL));
		}
	}
	return ins;
}

void
zDesInstance(struct ZObjInstance *ins)
{
	if(Z_HAVE_PARENT(ins))
		zDesInstance(Z_PARENT_OBJ(ins)); 
	if(ins->class->destructor)
		ins->class->destructor(ins);
}

void *
zGetClassByName(const char *self, const char *class_name)
{
	struct ZObjClass *dst_class = NULL;
	
	if((dst_class = find_class(self)) == NULL) {
		printf("CRITICAL: class %s not found.\n", self);
		return NULL;
	}
	
	if(class_name != NULL) {	
		while(dst_class != NULL && strcmp(dst_class->class_name, class_name) != 0)
			dst_class = dst_class->parent;
		if(dst_class == NULL) {
			printf("CRITICAL: class %s is not on inheritance chain of %s\n", class_name, self);
			return NULL;
		}
	}
	return getMemPtr(&dst_class->class_body, 0, 0);
}

void *
zGetClassByInstance(struct ZObjInstance *ins, const char *class_name)
{
	if(class_name == NULL)
		return getMemPtr(&ins->class->class_body, 0, 0);
	return zGetClassByName(ins->class->class_name, class_name);
}

void *
zGetInstance(struct ZObjInstance *ins, const char *class_name)
{
	if(class_name != NULL) {
		while(strcmp(ins->class->class_name, class_name) != 0) {
			if(ins->class->parent == NULL) {
				printf("CRITICAL: class %s not found.\n", class_name);
				return NULL;
			}
			ins = getMemPtr(&ins->parent, 0, 0);
		}
	}
	return getMemPtr(&ins->instance_body, 0, 0);
}

void
zDesInstanceSpace(struct ZObjInstance *ins)
{
	destroyMem(ins->instance_body);
	if(Z_HAVE_PARENT(ins)) {
		destroyMem(ins->parent);
	}
}

static void
dup_parent_class(struct ZObjClass *self)
{
	if(self->parent == NULL)
		return;
	struct ZObjClass *new_parent = malloc(sizeof(struct ZObjClass));
	assert(new_parent != NULL);
	memset(new_parent, 0, sizeof(struct ZObjClass));
	new_parent->class_name = self->parent->class_name;
	new_parent->parent = self->parent->parent;
	new_parent->constructor = self->parent->constructor;
	new_parent->destructor = self->parent->destructor;
	new_parent->interfaces = makeMem(getMemIndex(self->parent->interfaces));
	new_parent->class_body = makeMem(getMemIndex(self->parent->class_body));
	memCpy(&new_parent->class_body, getMemPtr(&self->parent->class_body, 0, 0),
			getMemIndex(self->parent->class_body));
	memCpy(&new_parent->interfaces, getMemPtr(&self->parent->interfaces, 0, 0),
			getMemIndex(self->parent->interfaces));
	self->parent = new_parent;
	dup_parent_class(self->parent);
}

int
zOverLoad(const char *self)
{
	struct ZObjClass *dst_class = NULL;
	
	if((dst_class = find_class(self)) == NULL) {
		printf("CRITICAL: class %s not found.\n", self);
		return -1;
	}
	dup_parent_class(dst_class);
	return 0;
}
