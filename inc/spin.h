#pragma once
#include <stdbool.h>

#define INIT_SPIN_LOCK { 0 }

typedef struct {
	_Atomic(int) lock;
} spin_lock_t;

void spin_lock(spin_lock_t *);
void spin_unlock(spin_lock_t *);
bool spin_try_lock(spin_lock_t *);
