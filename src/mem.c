#include "mem.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static inline void
assert_mem_capacity(mem_t_ptr mem_p, unsigned int need)
{
	if(mem_p->len < need) {
		assert(need < 1024 && need > 0);
		if(mem_p->mem_ptr == NULL) {
			mem_p->mem_ptr = malloc(need << 1);
		} else {
			mem_p->mem_ptr = realloc(mem_p->mem_ptr, need << 1);
		}
		assert(mem_p->mem_ptr != NULL);
		mem_p->len = need << 1;
	}
}

mem_t
makeMem(unsigned int init_size)
{
	if(init_size == 0)
		return (mem_t){ 0 };
	void *ptr = malloc(init_size);
	assert(ptr != NULL);
	memset(ptr, 0, init_size);
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
	if(mem.mem_ptr != NULL)
		free(mem.mem_ptr);
}		
