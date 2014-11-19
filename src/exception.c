/**************************************************************************
 * exception.c                                                            * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#include "exception.h"
#include "rb_tree.h"
#include "container.h"
#include "spin.h"
#include <setjmp.h>
#include <assert.h>
#include <string.h>

#define cur_thread (get_cur_thread ? get_cur_thread() : 0)

/** thread identifier. */
static unsigned long (*get_cur_thread)(void) = NULL;

/** implement muti thread safe. */
spin_lock_t spin_lk = INIT_SPIN_LOCK;

struct stack_node {
	jmp_buf jb;				/**< to save jmp_buf. */
	struct stack_node *prev;		/**< to push this struct on stack. */
};

/** stack info. */
struct stack_type {
	unsigned long thread_id;		/**< for miti thread safe. */
	struct stack_node *stack_top;
	struct ZObjInstance *cur_exception;	/**< current exception on this thread. */
	struct RBNode rb_node;			/**< to store this struct in red black tree. */
} ;

/** the red black tree */
static RBTree stack_tree;

/** translate rb_node addr to stack_type addr. */
#define to_stack(node) container_of(node, struct stack_type, rb_node)

static void *
get_thread_id(struct RBNode *node)
{
	return &to_stack(node)->thread_id;
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

static struct RBNode *
make_new_stack(void *id)
{
	struct stack_type *stack = malloc(sizeof(struct stack_type));
	stack->thread_id = *(unsigned long *)id;
	stack->stack_top = NULL;
	stack->cur_exception = NULL;
	return &stack->rb_node;
}

void
exceptionInit(unsigned long (*thread_identifier)(void))
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
	spinUnlock(&spin_lk);
	struct RBNode *stack = rbSearch(stack_tree, &thread_id);
	struct stack_node *new_node = malloc(sizeof(struct stack_node));
	assert(new_node != NULL);
	new_node->prev = to_stack(stack)->stack_top;
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
	free(node);
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
	to_stack(stack)->cur_exception = e;
	longjmp(to_stack(stack)->stack_top->jb, 1);
}
