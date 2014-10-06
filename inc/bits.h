#ifndef BITS_H
#define BITS_H

#define BITS_PER_BYTE 8

#define BITS_SET(ptr, index)					\
do {								\
	((char *)(ptr))[(index)/8] |= (0x01 << ((index)%8));	\
} while(0)

#define BITS_CLR(ptr, index)					\
do {								\
	((char *)(ptr))[(index)/8] &= ~(0x01 << ((index)%8));	\
} while(0)

#define BITS_TEST(ptr, index)					\
({								\
	(((char *)(ptr))[(index)/8] >> ((index)%8)) & 0x01;	\
})

#endif
