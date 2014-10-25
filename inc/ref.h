#pragma once

#include <stdatomic.h>

typedef struct RefNode {
	_Atomic(int) count;
	void (*releaseFunc)(struct RefNode *);
} ref_t;

ref_t makeRef(void (*)(ref_t *));
int refGet(ref_t *);
int refPut(ref_t *);
