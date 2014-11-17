#pragma once
#include <stdbool.h>

#define INIT_SPIN_LOCK { 0 }

typedef struct {
	_Atomic(int) lock;
} spin_lock_t;

void spinLock(spin_lock_t *);
void spinUnlock(spin_lock_t *);
bool spinTryLock(spin_lock_t *);
