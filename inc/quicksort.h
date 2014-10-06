#ifndef QUICKSORT_H
#define QUICKSORT_H

typedef int (*CMP_FUNC)(void *, void *);
void quickSort(void *, unsigned int, unsigned int, CMP_FUNC);

#endif
