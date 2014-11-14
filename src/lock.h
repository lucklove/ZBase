#pragma once

#define INIT_SPIN_LOCK { 0 }

typedef spin_lock_t {
	_Atomic(int) lock;
};

void spin_lock(spin_lock_t *);
void spin_unlock(spin_lock_t *);
