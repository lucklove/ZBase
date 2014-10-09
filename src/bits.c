#include "bits.h"

inline void
bits_set(void *ptr, unsigned int index)
{
	BITS_SET(ptr, index);
}

inline void
bits_clr(void *ptr, unsigned int index)
{
	BITS_CLR(ptr, index);
}

inline int
bits_test(void *ptr, unsigned int index)
{
	return BITS_TEST(ptr, index);
}
