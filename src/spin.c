#include "spin.h"
#include <stdatomic.h>

void
spinLock(spin_lock_t *lock)
{
	while(atomic_fetch_add(&lock->lock, 1) != 0) 
		atomic_fetch_sub(&lock->lock, 1);
}

bool
spinTryLock(spin_lock_t *lock)
{
	if(atomic_fetch_add(&lock->lock, 1) != 0) {
		atomic_fetch_sub(&lock->lock, 1);
		return false;
	}
	return true;
}

void
spinUnlock(spin_lock_t *lock)
{
	atomic_fetch_sub(&lock->lock, 1);
}
