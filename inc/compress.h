#pragma once

#include "rb_tree.h"
#include "hm_tree.h"

typedef struct {
	void *data;
	unsigned int bit_size;
	HMTree encode_tree;
} compress_t;

compress_t 	makeCompressType(void *, unsigned int);
void		destroyCompressType(compress_t);
compress_t 	compressEncode(compress_t);
compress_t	compressDecode(compress_t);
void		compressEncodeFile(const char *, const char *);
void		compressDecodeFile(const char *, const char *);
