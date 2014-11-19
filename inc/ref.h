/**************************************************************************
 * ref.h                                                                  * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include <stdatomic.h>

/** 
 * \brief Refrence type.
 * \note 
 * 	You should use it your struct and can use container_of(contianer.h) 
 * 	to access your own struct.
 * \example
 * 	struct MyStruct {
 * 		int some_val;
 * 		ref_t ref_member;
 * 	};
 * 	So the releaseFunc should like
 * 	void release(struct RefNode *ref) 
 * 	{
 * 		do_something_with(contianer_of(ref, struct MyStruct, ref_member));
 * 	}
 */
typedef struct RefNode {
	_Atomic(int) count;			/**< to make sure thread safe. */
	void (*releaseFunc)(struct RefNode *);	/**< free function provide by user. */
} ref_t;

/**
 * \brief Get a refrence type.
 * \param releaseFunc 
 * 	Free function provide by user, when the count arrive 0, this function
 * 	will be called.
 * \return A refrence type(which is a struct).
 */
ref_t makeRef(void (*releaseFunc)(ref_t *));

/**
 * \brief Inc refrence count atomicly.
 * \param ref Refrence pointer.
 * \return Current refrence count(after this operation).
 */
int refGet(ref_t *ref);

/**
 * \brief Dec refrence count atomicly.
 * \param ref Refrence pointer.
 * \return Current refrence count(after this operation).
 */
int refPut(ref_t *);
