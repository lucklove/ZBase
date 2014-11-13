#pragma once

#include "mem.h"

typedef struct {
	mem_t array;
	unsigned int row;
	unsigned int col;
} matrix_t;

matrix_t makeMatrix(unsigned int, unsigned int, void *);
unsigned int matrixGetRow(matrix_t);
unsigned int matrixGetCol(matrix_t);
void destroyMatrix(matrix_t);
void matrixSet(matrix_t *, unsigned int, unsigned int, double);
double matrixGet(matrix_t, unsigned int, unsigned int);
void dumpMatrix(matrix_t);
matrix_t matrixScale(matrix_t);
void matrixDelRow(matrix_t *, unsigned int);
void matrixDelCol(matrix_t *, unsigned int);
matrix_t matrixMult(matrix_t, matrix_t);
matrix_t matrixMultNum(matrix_t, double);
matrix_t matrixMinus(matrix_t, matrix_t);
matrix_t matrixAdd(matrix_t, matrix_t);
void *matrixGetArray(matrix_t);
