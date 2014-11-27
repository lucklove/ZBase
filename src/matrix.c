#include "matrix.h"
#include "mem.h"
#include <assert.h>
#include <stdio.h>

unsigned int
matrixGetCol(matrix_t matrix)
{
	return matrix.col;
}

unsigned int
matrixGetRow(matrix_t matrix)
{
	return matrix.row;
}

matrix_t
makeMatrix(unsigned int row, unsigned int col, void *init_data)
{
	matrix_t ret;
	ret.array = makeMem(row * col * sizeof(double));
	if(init_data != NULL)
		memCpy(&ret.array, init_data, row * col * sizeof(double));
	ret.row = row;
	ret.col = col;
	return ret;
}

void
destroyMatrix(matrix_t matrix)
{
	destroyMem(matrix.array);
}

void
matrixSet(matrix_t *matrix, unsigned int row, unsigned int col, double val)
{
	SET_TYPE_MEM(&matrix->array, double, col + row * matrix->col, val);
}

double
matrixGet(matrix_t matrix, unsigned int row, unsigned int col)
{
	return GET_TYPE_ITEM(&matrix.array, double, col + row * matrix.col);
}

void
matrixTranspose(matrix_t *matrix)
{
	matrix_t new = makeMatrix(matrixGetCol(*matrix), matrixGetRow(*matrix), NULL);
	for(int i = 0; i < matrixGetCol(*matrix); ++i) {
		for(int j = 0; j < matrixGetRow(*matrix); ++j)
			matrixSet(&new, i, j, matrixGet(*matrix, j, i));
	}
	destroyMatrix(*matrix);
	*matrix = new;
}

void
matrixDelRow(matrix_t *matrix, unsigned int row_num)
{
	for(int i = row_num + 1; i < matrixGetRow(*matrix); ++i) {
		for(int j = 0; j < matrixGetCol(*matrix); ++j)
			matrixSet(matrix, i - 1, j, matrixGet(*matrix, i, j));
	}
	--matrix->row;
}

void
matrixAddRow(matrix_t *matrix, unsigned int add_num)
{
	matrix->row += add_num;
}

void
matrixSwapRow(matrix_t *matrix, unsigned int src_row, unsigned int dst_row)
{
	double tmp = 0.0;
	for(int i = 0; i < matrixGetCol(*matrix); ++i) {
		tmp = matrixGet(*matrix, src_row, i);
		matrixSet(matrix, src_row, i, matrixGet(*matrix, dst_row, i));
		matrixSet(matrix, dst_row, i, tmp);
	}
}

void
matrixDelCol(matrix_t *matrix, unsigned int col_num)
{
	for(int i = 0; i < matrixGetRow(*matrix); ++i) {
		for(int j = col_num + 1; j < matrixGetCol(*matrix); ++j)
			matrixSet(matrix, i, j - 1, matrixGet(*matrix, i, j));
	}
	for(int i = 1; i < matrixGetRow(*matrix); ++i) {
		for(int j = 0; j < matrixGetCol(*matrix); ++j)
			matrixSet(matrix, i, j - i, matrixGet(*matrix, i, j));
	}
	--matrix->col;
}

void
matrixAddCol(matrix_t *matrix, unsigned int add_num)
{
	matrix_t new = makeMatrix(matrixGetRow(*matrix), matrixGetCol(*matrix) + add_num, NULL);
	for(int i = 0; i < matrixGetRow(*matrix); ++i) {
		for(int j = 0; j < matrixGetCol(*matrix); ++j)
			matrixSet(&new, i, j, matrixGet(*matrix, i, j));
	}
	destroyMatrix(*matrix);
	*matrix = new;
}
	
void
matrixSwapCol(matrix_t *matrix, unsigned int src_col, unsigned int dst_col)
{
	double tmp = 0.0;
	for(int i = 0; i < matrixGetRow(*matrix); ++i) {
		tmp = matrixGet(*matrix, i, src_col);
		matrixSet(matrix, i, src_col, matrixGet(*matrix, i, dst_col));
		matrixSet(matrix, i, dst_col, tmp);
	}
}

void
matrixSortRow(matrix_t *matrix, unsigned int col)
{
	for(int i = 0; i < matrixGetRow(*matrix) - 1; ++i) {
		for(int j = 0; j < matrixGetRow(*matrix) - i - 1; ++j) {
			if(matrixGet(*matrix, j, col) > matrixGet(*matrix, j + 1, col))
				matrixSwapRow(matrix, j, j + 1);
		}
	}
}

void
matrixSortCol(matrix_t *matrix, unsigned int row)
{
	for(int i = 0; i < matrixGetCol(*matrix) - 1; ++i) {
		for(int j = 0; j < matrixGetCol(*matrix) - i - 1; ++j) {
			if(matrixGet(*matrix, row, j) > matrixGet(*matrix, row, j + 1))
				matrixSwapCol(matrix, j, j + 1);
		}
	}
}

matrix_t
matrixMult(matrix_t m1, matrix_t m2)
{
	matrix_t ret = makeMatrix(matrixGetRow(m1), matrixGetCol(m2), NULL);
	assert(matrixGetCol(m1) == matrixGetRow(m2));
	for(int i = 0; i < matrixGetRow(m1); ++i) {
		for(int j = 0; j < matrixGetCol(m2); ++j) {
			double sum = 0.0;
			for(int k = 0; k < matrixGetCol(m1); ++k) {
				sum += matrixGet(m1, i, k) * matrixGet(m2, k, j);
			}
			matrixSet(&ret, i, j, sum);
		}
	}
	return ret;
}

matrix_t
matrixMultNum(matrix_t m, double num)
{
	matrix_t ret = makeMatrix(matrixGetRow(m), matrixGetCol(m), NULL);
	for(int i = 0; i < matrixGetRow(m); ++i) {
		for(int j = 0; j < matrixGetCol(m); ++j)
			matrixSet(&ret, i, j, matrixGet(m, i, j) * num);
	}
	return ret;
}

matrix_t
matrixMinus(matrix_t m1, matrix_t m2)
{
	matrix_t ret = makeMatrix(matrixGetRow(m1), matrixGetCol(m2), NULL);
	for(int i = 0; i < matrixGetRow(m1); ++i) {
		for(int j = 0; j < matrixGetCol(m1); ++j)
			matrixSet(&ret, i, j, matrixGet(m1, i, j) - matrixGet(m2, i, j));
	}
	return ret;
}

matrix_t
matrixAdd(matrix_t m1, matrix_t m2)
{
	matrix_t ret = makeMatrix(matrixGetRow(m1), matrixGetCol(m2), NULL);
	for(int i = 0; i < matrixGetRow(m1); ++i) {
		for(int j = 0; j < matrixGetCol(m1); ++j)
			matrixSet(&ret, i, j, matrixGet(m1, i, j) + matrixGet(m2, i, j));
	}
	return ret;
}

void *
matrixGetArray(matrix_t matrix)
{
	return getMemPtr(&matrix.array, 0, 0);
}

matrix_t
matrixDotDiv(matrix_t m1, matrix_t m2)
{
	if(matrixGetRow(m1) == 0 || matrixGetRow(m2) == 0 || matrixGetCol(m1) == 0 || matrixGetCol(m2))
		return makeMatrix(0, 0, NULL);
	assert(matrixGetRow(m1) == matrixGetRow(m2));
	assert(matrixGetCol(m1) == matrixGetCol(m2));
	matrix_t ret = makeMatrix(matrixGetRow(m1), matrixGetCol(m1), NULL);
	for(int i = 0; i < matrixGetRow(m1); ++i) {
		for(int j = 0; j < matrixGetCol(m1); ++j) {
			matrixSet(&ret, i, j, 
				matrixGet(m1, i, j) / matrixGet(m2, i, j));
		}
	}
	return ret;
}

matrix_t
matrixDotMult(matrix_t m1, matrix_t m2)
{
	if(matrixGetRow(m1) == 0 || matrixGetRow(m2) == 0 || matrixGetCol(m1) == 0 || matrixGetCol(m2))
		return makeMatrix(0, 0, NULL);
	assert(matrixGetRow(m1) == matrixGetRow(m2));
	assert(matrixGetCol(m1) == matrixGetCol(m2));
	matrix_t ret = makeMatrix(matrixGetRow(m1), matrixGetCol(m1), NULL);
	for(int i = 0; i < matrixGetRow(m1); ++i) {
		for(int j = 0; j < matrixGetCol(m1); ++j) {
			matrixSet(&ret, i, j, 
				matrixGet(m1, i, j) * matrixGet(m2, i, j));
		}
	}
	return ret;
}

matrix_t
matrixMergeRow(matrix_t m1, matrix_t m2)
{
	if(matrixGetRow(m1) == 0 || matrixGetCol(m1) == 0)
		return makeMatrix(matrixGetRow(m2), matrixGetCol(m2), matrixGetArray(m2));
	if(matrixGetRow(m1) == 0 || matrixGetCol(m2) == 0)
		return makeMatrix(matrixGetRow(m1), matrixGetCol(m1), matrixGetArray(m1));
	assert(matrixGetCol(m1) == matrixGetCol(m2));
	matrix_t ret = makeMatrix(matrixGetRow(m1), matrixGetCol(m1), matrixGetArray(m1));
	memCat(&ret.array, matrixGetArray(m2), matrixGetRow(m2) * matrixGetCol(m2) *sizeof(double));
	ret.row += matrixGetRow(m2);
	return ret;
}

matrix_t
matrixMergeCol(matrix_t m1, matrix_t m2)
{
	if(matrixGetRow(m1) == 0 || matrixGetCol(m1) == 0)
		return makeMatrix(matrixGetRow(m2), matrixGetCol(m2), matrixGetArray(m2));
	if(matrixGetRow(m1) == 0 || matrixGetCol(m2) == 0)
		return makeMatrix(matrixGetRow(m1), matrixGetCol(m1), matrixGetArray(m1));
	assert(matrixGetRow(m1) == matrixGetRow(m2));
	matrix_t ret = makeMatrix(matrixGetRow(m1), matrixGetCol(m1), matrixGetArray(m1));
	matrixAddCol(&ret, matrixGetCol(m2));
	for(int i = 0; i < matrixGetRow(m1); ++i) {
		for(int j = matrixGetCol(m1); j < matrixGetCol(ret); ++j)
			matrixSet(&ret, i, j, matrixGet(m2, i, j - matrixGetCol(m1)));
	}
	return ret;
}

void
dumpMatrix(matrix_t matrix)
{
	for(int i = 0; i < matrixGetRow(matrix); ++i) {
		for(int j = 0; j < matrixGetCol(matrix); ++j)
			printf("%lf\t", matrixGet(matrix, i, j));
		printf("\n");
	}
}	
