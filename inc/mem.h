/**************************************************************************
 * mem.h                                                                  * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#define INIT_MEM 0			/**< default init mem size. */

/** store mem infomation. */
typedef struct {
	void *mem_ptr;			/**< user data stored at here. */
	unsigned int index;		/**< used mem. */
	unsigned int len;		/**< total mem. */
} mem_t, *mem_t_ptr;

/** make a new mem whose init size is init_size. */
mem_t makeMem(unsigned int init_size);

/**
 * \brief Get a memory address in mem_t.
 * \param mem_p Addr of a mem_t type variable, from where the memory address take out.
 * \param index The index of dest address in mem_p->mem_ptr.
 * \param size 
 * 	The user's type's size, for example, if you want to translate the return to
 * 	int *, the size should be sizeof(int).
 * 	This param is given for assume the mem_ptr is long enough for user to use.
 * \example int x = *(int *)getMemPtr(mem_p, 3, sizeof(int));
 */
void *getMemPtr(mem_t_ptr mem_p, unsigned int index, unsigned int size);

/** copy data whose length is len to the mem mem_p point. */
void memCpy(mem_t_ptr mem_p, const void *data, unsigned int data_len);

/** cat data whose length is len to the mem mem_p point. */
void memCat(mem_t_ptr mem_p, const void *data, unsigned int len);

/** set mem_p->mem_ptr[index] to mem_p->mem_ptr[index+len] as val. */
void memSet(mem_t_ptr mem_p, char val, unsigned int len);

/** change mem_p->index to new_index. */
void setMemIndex(mem_t_ptr mem_p, unsigned int new_index);

/** get mem.index. */
unsigned int getMemIndex(mem_t mem);

/** free mem.mem_ptr. */
void destroyMem(mem_t mem);

#ifdef MEM_DEBUG

#include <stdio.h>

/** if defined MEM_DEBUG, this must be called before any mem_t operation. */
void memDebugInit(void);

/** check if there is any mem leak. */
void memCheckLeak(FILE *);

/** release debug source(red black tree mainly). */
void memDebugRelease(void);

#else

/** if undef MEM_DEBUG, these can also be used. */
#define memDebugInit()
#define memCheckLeak(f)
#define memDebugRelease()

#endif

/**
 * \brief Get item number in a mem_t type.
 * \param mem_t A mem_t type variable.
 * \item_type Item type in mem_t.
 */
#define GET_ITEM_NUM(mem_t, item_type) (getMemIndex(mem_t) / (unsigned int)sizeof(item_type))

/**
 * \brief Add an item on the end of mem_t.
 * \param mem_t_ptr A refrence to a mem_t type variable.
 * \param typp The val type.
 * \param val The val you add.
 */
#define  ADD_ITEM(mem_t_ptr, type, val) 	\
	SET_TYPE_MEM(mem_t_ptr, type, GET_ITEM_NUM(*(mem_t_ptr), type), val)

/**
 * \brief Get a refrence to an item in a mem_t type variable.
 * \param mem_t_ptr Refrece to a mem_t type.
 * \param type The item type in dest mem_t type.
 * \param _index The index of the item in mem.
 */
#define GET_TYPE_MEM(mem_t_ptr, type, _index) ((type *)getMemPtr((mem_t_ptr), sizeof(type) * (_index), sizeof(type)))

/** \see macro GET_TYPE_MEM. */
#define GET_TYPE_ITEM(mem_t_ptr, type, _index) (*(GET_TYPE_MEM(mem_t_ptr, type, _index)))

/**
 * \brief Set a val in mem.
 * \param mem_t_ptr Point to the mem you want to set.
 * \param type Item type in mem.
 * \param val The val to be set.
 */
#define SET_TYPE_MEM(mem_t_ptr, type, _index, val)				\
do {										\
	*GET_TYPE_MEM(mem_t_ptr, type, _index) = val;				\
	if((mem_t_ptr)->index < sizeof(type) * (_index + 1))			\
		setMemIndex(mem_t_ptr, sizeof(type) * (_index + 1));		\
} while(0)
