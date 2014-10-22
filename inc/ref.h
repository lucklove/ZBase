#pragma once

#include <stdatomic.h>

typedef struct {
	_Atomic(int) count;
	void (*releaseFunc)(void *);
} ref_t;

ref_t makeRef(void (*)(ref_t *));
int refGet(ref_t *);
int refPut(ref_t *);
