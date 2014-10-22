#pragma once

typedef int (*CMP_FUNC)(void *, void *);
void quickSort(void *, unsigned int, unsigned int, CMP_FUNC);
