#pragma once

#define INIT_MEM 1

typedef struct {
	void *mem_ptr;
	unsigned int index;
	unsigned int len;
} mem_t, *mem_t_ptr;

mem_t makeMem(unsigned int);
void *getMemPtr(mem_t_ptr, unsigned int, unsigned int);
void memCpy(mem_t_ptr, const void *, unsigned int);
void memCat(mem_t_ptr, const void *, unsigned int);
void memSet(mem_t_ptr, char, unsigned int);
void setMemIndex(mem_t_ptr, unsigned int);
unsigned int getMemIndex(mem_t mem);
void destroyMem(mem_t);

#ifdef MEM_DEBUG

#include <stdio.h>

void memDebugInit(void);
void memCheckLeak(FILE *);
void memDebugRelease(void);

#else

#define memDebugInit()
#define memCheckLeak(f)
#define memDebugRelease()

#endif

#define GET_ITEM_NUM(mem_t, item_type) (getMemIndex(mem_t) / (unsigned int)sizeof(item_type))

#define  ADD_ITEM(mem_t_ptr, type, val) 	\
	SET_TYPE_MEM(mem_t_ptr, type, GET_ITEM_NUM(*(mem_t_ptr), type), val)

#define GET_TYPE_MEM(mem_t_ptr, type, _index) ((type *)getMemPtr((mem_t_ptr), sizeof(type) * (_index), sizeof(type)))

#define GET_TYPE_ITEM(mem_t_ptr, type, _index) (*(GET_TYPE_MEM(mem_t_ptr, type, _index)))

#define SET_TYPE_MEM(mem_t_ptr, type, _index, val)				\
do {										\
	*GET_TYPE_MEM(mem_t_ptr, type, _index) = val;				\
	if((mem_t_ptr)->index < sizeof(type) * (_index + 1))			\
		setMemIndex(mem_t_ptr, sizeof(type) * (_index + 1));		\
} while(0)
