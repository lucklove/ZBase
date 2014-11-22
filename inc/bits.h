/**************************************************************************
 * bits.h                                                                 * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 * implemented in bits.c						  *
 **************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** don's ask why. */
#define BITS_PER_BYTE 8

/**
 * \biref Set dest bit.
 * \param ptr The buffer which hold dest bit.
 * \param offset The offset of dest bit in buffer ptr point to.
 */
void bits_set(void *ptr, unsigned int offset);

/**
 * \brief Clr dest bit.
 * \param ptr Same with bits_set.
 * \param offset Same with bits_clr.
 */
void bits_clr(void *ptr, unsigned int offset);

/**
 * \brief Test dest bit.
 * \param ptr Same with bits_set.
 * \param offset Same with bits_set.
 * \return If dest bit is seted, 1, else, 0.
 */
int bits_test(void *ptr, unsigned int offset);

/**
 * \biref Print dest buffer's binary val.
 * \param ptr Same with bits_set.
 * \param len The buffer's len in byte.
 */
void bits_dump(void *ptr, unsigned int len);

#ifdef __cplusplus
}
#endif
