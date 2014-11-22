#pragma once

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	mem_t mem;
} base64_t;

base64_t makeBase64Type(void *, unsigned int);
base64_t base64Encode(base64_t);
base64_t base64Decode(base64_t);
void destroyBase64Type(base64_t);
void *getBase64Ptr(base64_t);
unsigned int getBase64Len(base64_t);
void base64EncodeFile(const char *, const char *);
void base64DecodeFile(const char *, const char *);

#ifdef __cplusplus
}
#endif
