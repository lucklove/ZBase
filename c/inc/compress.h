/**************************************************************************
 * compress.h                                                             * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 * implemented in compress.c                                              *
 **************************************************************************/

#pragma once

#include "rb_tree.h"
#include "hm_tree.h"

/** store compress information. */
typedef struct {
	void *data;			/**< meta data. */
	unsigned int bit_size;		/**< length of data by bit. */
	/**< a Huffman tree which store compress information. */
	HMTree encode_tree;
} compress_t;

/**
 * \brief Generate a compress handle(compress_t).
 * \param data Initial data to be encode/decode.
 * \param data_len Length of data.
 * \return A compress_t handle, which will be used after this.
 */ 
compress_t 	makeCompressType(void *data, unsigned int data_len);

/**
 * \brief Release dest compress_t's resource.
 * \param cp Dest compress_t.
 */ 
void		destroyCompressType(compress_t cp);

/**
 * \brief Encode data in a compress_t.
 * \param cp Dest compress_t.
 * \return Another compress_t which has data encoded.
 */
compress_t 	compressEncode(compress_t cp);

/**
 * \brief Decode data in a compress_t.
 * \param cp The compress_t which has data to be decoded.
 * \return Another compress_t which has data decoded.
 * \warning The param cp should  contain an encode tree.
 */ 
compress_t	compressDecode(compress_t cp);

/**
 * \brief Compress a file.
 * \param src Input file.
 * \param dst Output file.
 */ 
void		compressEncodeFile(const char *src, const char *dst);

/**
 * \brief Decompress a file.
 * \param src Input file.
 * \param dst Output file.
 */ 
void		compressDecodeFile(const char *src, const char *dst);
