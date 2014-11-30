/**************************************************************************
 * base64.h                                                               * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 * implemented in base64.c                                                *
 **************************************************************************/

#pragma once

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

/** information about encode/decode message. */
typedef struct {
	mem_t mem;					/**< memory. */
} base64_t;

/**
 * \brief Generate a base64 handle(base64_t).
 * \param data Initial data to be encode/decode.
 * \param data_len Length of data.
 * \return A base64_t handle, which will be used after this.
 */ 
base64_t makeBase64Type(void *data, unsigned int data_len);

/**
 * \brief Encode data in a base64_t.
 * \param b_64 The base64_t which has data to be encoded.
 * \return Another base64_t which has data encoded.
 */ 
base64_t base64Encode(base64_t b_64);

/**
 * \brief Decode data in a base64_t.
 * \param b_64 The base64_t which has data to be decoded.
 * \return Another base64_t which has data decoded.
 */ 
base64_t base64Decode(base64_t b_64);

/**
 * \brief Release dest base64_t's resource.
 * \param b_64 Dest base64_t.
 */ 
void destroyBase64Type(base64_t b_64);

/**
 * \brief Get the data in base64_t.
 * \param b_64 Dest base64_t.
 */ 
void *getBase64Ptr(base64_t b_64);

/**
 * \brief Get data's length in base64_t.
 * \param b_64 Dest base64_t.
 */ 
unsigned int getBase64Len(base64_t b_64);

/**
 * \brief Encode a file use base64.
 * \param src Input file.
 * \param dst Output file.
 */ 
void base64EncodeFile(const char *src, const char *dst);

/**
 * \brief Decode a file use base64.
 * \param src Input file.
 * \param dst Output file.
 */ 
void base64DecodeFile(const char *src, const char *dst);

#ifdef __cplusplus
}
#endif
