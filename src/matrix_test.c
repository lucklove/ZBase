#include "matrix.h"

int
main(int argc, char *argv[])
{
	double v[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	matrix_t m = makeMatrix(2, 4, v);
	dumpMatrix(m);
	matrix_t n = matrixMultNum(m, 3);
	dumpMatrix(n);
	return 0;
}
