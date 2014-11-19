/**************************************************************************
 * exception.c                                                            * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#include "zobject.h"
#include "rb_tree.h"
#include "container.h"
#include "spin.h"
#include <setjmp.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>


#include <stdio.h>

#define cur_thread (get_cur_thread ? get_cur_thread() : 0)

/** thread identifier. */
static unsigned long (*get_cur_thread)(void) = NULL;

/** implement muti thread safe. */
spin_lock_t spin_lk = INIT_SPIN_LOCK;

struct stack_node {
	jmp_buf jb;				/**< to save jmp_buf. */
	struct stack_node *prev;		/**< to push this struct on stack. */
	RBTree instance_tree;			/**< to save instance initialized in try block. */	
};

/** stack info. */
struct stack_type {
	unsigned long thread_id;		/**< for miti thread safe. */
	struct stack_node *stack_top;
	struct ZObjInstance *cur_exception;	/**< current exception on this thread. */
	struct RBNode rb_node;			/**< to store this struct in red black tree. */
} ;

/** instance info, to record instance initialized in try block. */
struct instance_info {
	struct ZObjInstance *instance;
	struct RBNode rb_node;
};

/** the red black tree */
static RBTree stack_tree;

/** translate rb_node addr to stack_type addr. */
#define to_stack(node) container_of(node, struct stack_type, rb_node)

/** translate rb_node addr to instance_info addr. */
#define to_instance_info(node) container_of(node, struct instance_info, rb_node)

static void *
get_thread_id(struct RBNode *node)
{
	return &to_stack(node)->thread_id;
}

static void *
get_instance_addr(struct RBNode *node)
{
	return to_instance_info(node)->instance;
}

static int
cmp_thread_id(void *i1, void *i2)
{
	if(*(unsigned long *)i1 > *(unsigned long *)i2) {
		return 1;
	} else if(*(unsigned long *)i1 < *(unsigned long *)i2) {
		return -1;
	} else {
		return 0;
	}
}

static int
cmp_instance_addr(void *a1, void *a2)
{
	if(a1 > a2) {
		return 1;
	} else if(a1 < a2) {
		return -1;
	} else {
		return 0;
	}
}

static struct RBNode *
make_new_stack(void *id)
{
	struct stack_type *stack = malloc(sizeof(struct stack_type));
	stack->thread_id = *(unsigned long *)id;
	stack->stack_top = NULL;
	stack->cur_exception = NULL;
	return &stack->rb_node;
}

static struct RBNode *
make_new_instance_info(void *instance_addr)
{
	struct instance_info *info = malloc(sizeof(struct instance_info));
	info->instance = instance_addr;
	return &info->rb_node;
}

static void
release_instance_info(struct RBNode *node)
{
	struct ZObjInstance *instance = to_instance_info(node)->instance;
	printf("release\n");
	zDesInstance(instance);
	free(to_instance_info(node));
}
	
void
__exception_init(unsigned long (*thread_identifier)(void))
{
	get_cur_thread = thread_identifier;
	stack_tree = makeRBTree(get_thread_id, cmp_thread_id, make_new_stack, NULL);
}

void *
__push_jmp_point()
{
	unsigned long thread_id = cur_thread;
	spinLock(&spin_lk);
	rbInsert(&stack_tree, &thread_id);
	struct RBNode *stack = rbSearch(stack_tree, &thread_id);
	spinUnlock(&spin_lk);
	struct stack_node *new_node = malloc(sizeof(struct stack_node));
	assert(new_node != NULL);
	new_node->prev = to_stack(stack)->stack_top;
	new_node->instance_tree = makeRBTree(get_instance_addr, cmp_instance_addr,
					make_new_instance_info, NULL);					
	to_stack(stack)->stack_top = new_node;
	return new_node->jb;
}

void
__pop_jmp_point()
{
	unsigned long thread_id = cur_thread;
	spinLock(&spin_lk);
	struct RBNode *stack = rbSearch(stack_tree, &thread_id);
	spinUnlock(&spin_lk);
	struct stack_node *node = to_stack(stack)->stack_top;
	assert(node != NULL);
	to_stack(stack)->stack_top = node->prev;
	rbSetReleaseFunc(&node->instance_tree, release_instance_info);
	destroyRBTree(node->instance_tree);
	free(node);
}	

void
__exception_insert_instance(struct ZObjInstance *instance)
{
	unsigned long thread_id = cur_thread;
	spinLock(&spin_lk);
	struct RBNode *stack = rbSearch(stack_tree, &thread_id);
	spinUnlock(&spin_lk);
	if(stack == NULL)
		return;
	struct stack_node *node = to_stack(stack)->stack_top;
	rbInsert(&node->instance_tree, instance);
}

void
__exception_delete_instance(struct ZObjInstance *instance)
{
	unsigned long thread_id = cur_thread;
	spinLock(&spin_lk);
	struct RBNode *stack = rbSearch(stack_tree, &thread_id);
	spinUnlock(&spin_lk);
	if(stack == NULL)
		return;
	struct stack_node *node = to_stack(stack)->stack_top;
	rbDelete(&node->instance_tree, instance);
}

struct ZObjInstance *
__get_cur_exception()
{
	unsigned long thread_id = cur_thread;
	spinLock(&spin_lk);
	struct RBNode *stack = rbSearch(stack_tree, &thread_id);
	spinUnlock(&spin_lk);
	assert(to_stack(stack)->cur_exception != NULL);
	return to_stack(stack)->cur_exception;
}

void
__throw(struct ZObjInstance *e)
{
	unsigned long thread_id = cur_thread;
	spinLock(&spin_lk);
	struct RBNode *stack = rbSearch(stack_tree, &thread_id);
	spinUnlock(&spin_lk);
	zObjIncRef(e);
	to_stack(stack)->cur_exception = e;
	zObjDecRef(e);
	longjmp(to_stack(stack)->stack_top->jb, 1);
}
