#pragma once

#include <stdlib.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#ifdef __GNUC__
#define member_type(type, member) __typeof__(((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#define container_of(ptr, type, member) \
	((type *)((char *)(member_type(type, member) *){ptr} - offsetof(type, member))) 	
