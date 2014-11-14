#include "spin.h"
#include <stdatomic.h>

void
spin_lock(spin_lock_t *lock)
{
	while(atomic_fetch_add(&lock->lock, 1) != 0)
		atomic_fetch_sub(&lock->lock, 1);
}

void
spin_unlock(spin_lock_t *lock)
{
	atomic_fetch_sub(&lock->lock, 1);
}
