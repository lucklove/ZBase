/**************************************************************************
 * container.h                                                            * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include <stdlib.h>

/** caculator the offset of a struct membe in the struct. */
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/** for pointer type safe. */
#ifdef __GNUC__
#define member_type(type, member) __typeof__(((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

/**
 * \brief traslate ptr to it's container type *.
 * \param ptr the ptr which is to be translated.
 * \param type the container type.
 * \param member the member name of ptr in type.
 */
#define container_of(ptr, type, member) \
	((type *)((char *)(member_type(type, member) *){ptr} - offsetof(type, member))) 	
