/**************************************************************************
 * spin.h                                                                 * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once
#include <stdbool.h>

#define INIT_SPIN_LOCK { 0 }

/** spin lock type. */
typedef struct {
	_Atomic(int) lock;	/**< use atomic to perform lock function. */
} spin_lock_t;

/**
 * \brief Get a lock, if faild, block and try until success.
 * \param lock The lock variable user defined.
 */
void spinLock(spin_lock_t *lock);

/**
 * \brief Put a lock.
 * \param lock The lock variable user defined.
 */
void spinUnlock(spin_lock_t *lock);

/**
 * \brief Try to get a lock without blocking.
 * \param lock The lock variable user defined.
 * \return if success, true, else, false.
 */
bool spinTryLock(spin_lock_t *lock);
