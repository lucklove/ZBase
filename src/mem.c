#include "mem.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef MEM_DEBUG
#include <stdio.h>
#include "rb_tree.h"
#include "container.h"

struct mem_debug_node {
	void *ptr;
	struct RBNode rb_node;
};

static RBTree mem_debug_tree;

static void *
get_ptr(struct RBNode *p) 
{
        return container_of(p, struct mem_debug_node, rb_node)->ptr;
}

int
cmp_ptr(void *p1, void *p2)
{
        return (char *)p1 - (char *)p2;
}

struct RBNode *
make_debug_node(void *ptr)
{
        struct mem_debug_node *tmp = malloc(sizeof(struct mem_debug_node));
        tmp->ptr = ptr;
        return &tmp->rb_node;
}

void
free_debug_node(struct RBNode *node)
{
        free(container_of(node, struct mem_debug_node, rb_node));
}

static void
show_debug_node_tree(struct RBNode* node, FILE *debug_file){
        if(node == NULL)
                return;
        fprintf(debug_file, "mem %p not freed\n", container_of(node, struct mem_debug_node, rb_node)->ptr);
        if(node->rb_left != NULL)
                show_debug_node_tree(node->rb_left, debug_file);
        if(node->rb_right != NULL)
                show_debug_node_tree(node->rb_right, debug_file);
}

void
memDebugInit()
{
	mem_debug_tree = makeRBTree(get_ptr, cmp_ptr, make_debug_node, free_debug_node);
}

void
memCheckLeak(FILE *debug_file)
{
	show_debug_node_tree(mem_debug_tree.rb_node, debug_file);
}
	
void
memDebugRelease()
{
	destroyRBTree(mem_debug_tree);
}

#endif

#ifdef MEM_DEBUG
static inline void
assert_mem_capacity(mem_t_ptr mem_p, unsigned int need)
{
	if(mem_p->len < need) {
		void *old_ptr = mem_p->mem_ptr;
		if(mem_p->mem_ptr == NULL) {
			mem_p->mem_ptr = malloc(need << 1);
		} else {
			mem_p->mem_ptr = realloc(mem_p->mem_ptr, need << 1);
		}
		assert(mem_p->mem_ptr != NULL);
		mem_p->len = need << 1;
		if(old_ptr != mem_p->mem_ptr) {
			rbDelete(&mem_debug_tree, old_ptr);
			rbInsert(&mem_debug_tree, mem_p->mem_ptr);
		}
	}
}
#else
static inline void
assert_mem_capacity(mem_t_ptr mem_p, unsigned int need)
{
	if(mem_p->len < need) {
		if(mem_p->mem_ptr == NULL) {
			mem_p->mem_ptr = malloc(need << 1);
		} else {
			mem_p->mem_ptr = realloc(mem_p->mem_ptr, need << 1);
		}
		assert(mem_p->mem_ptr != NULL);
		mem_p->len = need << 1;
	}
}
#endif

mem_t
makeMem(unsigned int init_size)
{
	if(init_size == 0)
		return (mem_t){ 0 };
	void *ptr = malloc(init_size);
	assert(ptr != NULL);
	memset(ptr, 0, init_size);
#ifdef MEM_DEBUG
	rbInsert(&mem_debug_tree, ptr);
#endif
	return (mem_t){ ptr, 0, init_size };
}

void *
getMemPtr(mem_t_ptr mem_p, unsigned int index, unsigned int size)
{
	assert_mem_capacity(mem_p, index + size);
	return (char *)mem_p->mem_ptr + index;
}

void
setMemIndex(mem_t_ptr mem_p, unsigned int index)
{
	assert_mem_capacity(mem_p, index + 1);
	mem_p->index = index;
}

unsigned int
getMemIndex(mem_t mem)
{
	return mem.index;
}
	
void
memCpy(mem_t_ptr mem_p, const void *src, unsigned int len)
{
	assert_mem_capacity(mem_p, len);
	memcpy(mem_p->mem_ptr, src, len);
	mem_p->index = len;
}

void
memSet(mem_t_ptr mem_p, char val, unsigned int len)
{
	assert_mem_capacity(mem_p, len + mem_p->index);
	memset((char *)mem_p->mem_ptr + mem_p->index, val, len);
	mem_p->index += len;
}

void
memCat(mem_t_ptr mem_p, const void *src, unsigned int len)
{
	assert_mem_capacity(mem_p, len + mem_p->index);
	memcpy((char *)mem_p->mem_ptr + mem_p->index, src, len);
	mem_p->index += len;
}

void
destroyMem(mem_t mem)
{
#ifdef MEM_DEBUG
	if(rbSearch(mem_debug_tree, mem.mem_ptr) == NULL) {
		fprintf(stderr, "MEM ERROR: try to free %p which is NOT exist\n", mem.mem_ptr);
		return;
	}
	rbDelete(&mem_debug_tree, mem.mem_ptr);
#endif
	if(mem.mem_ptr != NULL) 
		free(mem.mem_ptr);
}		
