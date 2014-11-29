/**************************************************************************
 * matrix.h                                                               * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 * implemented in matrix.c                                                *
 **************************************************************************/

#pragma once

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

/** to store matrix infomation. */
typedef struct {
	mem_t array;		/**< matrix data. */
	unsigned int row;	/**< row number. */
	unsigned int col;	/**< col number. */
} matrix_t;			/**< matrix type. */

/**
 * \brief Initialization a matrix.
 * \param row The row number of the matrix.
 * \param col The col number of the matrix.
 * \param data 
 * 	Used to set the matrix data, if you want to 
 * 	initialize an empty matrix, set data as NULL.
 * \return The matrix initialized.
 */ 
matrix_t makeMatrix(unsigned int row, unsigned int col, void *data);

/** get row number of a matrix. */
unsigned int matrixGetRow(matrix_t matrix);

/** get col number of a matrix. */
unsigned int matrixGetCol(matrix_t matrix);

/** release matrix's resource. */
void destroyMatrix(matrix_t matrix);

/**
 * \brief Set matrix's val at dest postion.
 * \param matrix The pointer to dest matrix.
 * \param row_n Row number of dest postion.
 * \param col_n Col number of dest postion.
 * \val The value to be set.
 * \note Postions begin at 0.
 */ 
void matrixSet(matrix_t *matrix, 
	unsigned int row_n, unsigned int col_n, double val);

/** 
 * \brief Get a value in dest matrix at dest postion.
 * \param matrix The dest matrix.
 * \param row_n col_n Like matrixSet.
 * \return The value at dest postion.
 */ 
double matrixGet(matrix_t matrix, 
	unsigned int row_n, unsigned int col_n);

/**
 * \brief Print out all postion's values of dest matrix in matrix format.
 * \param matrix Dest matrix.
 */ 
void dumpMatrix(matrix_t matrix);

/**
 * \brief Transpose dest matrix.
 * \param matrix Pointer point to dest matrix.
 */ 
void matrixTranspose(matrix_t *matrix);

/**
 * \brief Delete a row in dest matrix.
 * \param matrix Pointer point to dest matrix.
 * \param row_n Dest row number.
 * \note Row number begin at 0.
 */ 
void matrixDelRow(matrix_t *matrix, unsigned int row_n);

/**
 * \brief Delete a col in dest matrix.
 * \param matrix col_n Like matrixDelRow.
 */  
void matrixDelCol(matrix_t *matrix, unsigned int col_n);

/**
 * \brief Mult two matrix.
 * \param lm Left operand.
 * \param rm Right operand.
 * \return The result.
 */ 
matrix_t matrixMult(matrix_t lm, matrix_t rm);

/**
 * \brief Mult a matrix with a number.
 * \param matrix The matrix and left operand.
 * \param number The number and right operand.
 * \return The result, a new matrix.
 */ 
matrix_t matrixMultNum(matrix_t matrix, double number);

/**
 * \brief Dot Div two matrix.
 * \param lm rm Like matrixMult.
 * \return Like matrixMult.
 * \note 
 * 	If one of the matrix is empty, an empty matrix
 * 	will be returned.
 */ 
matrix_t matrixDotDiv(matrix_t lm, matrix_t rm);

/**
 * \brief Dot Mult two matrix. 
 * \param lm rm Like matrixMult.
 * \return Like matrixMult.
 * \note Like matrixDotDiv.
 */
matrix_t matrixDotMult(matrix_t lm, matrix_t rm);

/**
 * \brief Minus two matrix.
 * \param lm rm Like matrixMult.
 * \return Like matrixMult.
 */ 
matrix_t matrixMinus(matrix_t lm, matrix_t rm);

/**
 * \brief Add two matrix.
 * \see matrixMinus.
 */ 
matrix_t matrixAdd(matrix_t, matrix_t);

/**
 * \brief Merge two matrix by row.
 * \param lm rm The two matrix to be merged.
 * \return A new matrix which is merged by lm and rm.
 */ 
matrix_t matrixMergeRow(matrix_t lm, matrix_t rm);

/**
 * \see matrixMergeRow.
 */ 
matrix_t matrixMergeCol(matrix_t, matrix_t);

/**
 * \brief Add a number of rows for a matrix.
 * \param matrix The dest matrix.
 * \param number The number of rows to be added.
 */ 
void matrixAddRow(matrix_t *matrix, unsigned int number);

/**
 * \see matrixAddRow.
 */
void matrixAddCol(matrix_t *matrix, unsigned int number);

/** get the array(which store data) in a matrix. */
void *matrixGetArray(matrix_t matrix);

#ifdef __cplusplus
}
#endif
