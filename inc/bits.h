#pragma once

#define BITS_PER_BYTE 8

#define BITS_SET(ptr, index)					\
({								\
	((char *)(ptr))[(index)/8] |= (0x01 << ((index)%8));	\
})

#define BITS_CLR(ptr, index)					\
({								\
	((char *)(ptr))[(index)/8] &= ~(0x01 << ((index)%8));	\
})

#define BITS_TEST(ptr, index)					\
({								\
	(((char *)(ptr))[(index)/8] >> ((index)%8)) & 0x01;	\
})

void bits_set(void *, unsigned int);
void bits_clr(void *, unsigned int);
void bits_dump(void *, unsigned int);
int bits_test(void *, unsigned int);
