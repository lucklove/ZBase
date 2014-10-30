#include "matrix.h"
#include "mem.h"

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

matrix_t
matrixScale(matrix_t matrix)
{
	matrix_t ret = makeMatrix(matrixGetCol(matrix), matrixGetRow(matrix), NULL);
	for(int i = 0; i < matrixGetCol(matrix); ++i) {
		for(int j = 0; j < matrixGetRow(matrix); ++j)
			matrixSet(&ret, i, j, matrixGet(matrix, j, i));
	}
	return ret;
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
dumpMatrix(matrix_t matrix)
{
	for(int i = 0; i < matrixGetRow(matrix); ++i) {
		for(int j = 0; j < matrixGetCol(matrix); ++j)
			printf("%lf\t", matrixGet(matrix, i, j));
		printf("\n");
	}
}	
