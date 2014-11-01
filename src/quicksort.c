#include "quicksort.h"
#include <string.h>

void
quickSort(void *data, unsigned int size, unsigned int num, int (*cmp_func)(const void *, const void *))
{
	char key[size];
	if(num < 2)
		return;
	memcpy(key, data, size);
	int k_index = 0;
	for(int i = 0, j = num - 1; i < j;) {
		while(cmp_func(data + size * j, key) >= 0 && j > k_index) j--;
		memcpy(data + size * k_index, data + size * j, size);
		k_index = j;
		memcpy(data + k_index * size, key, size);
		while(cmp_func(data + size * i, key) <= 0 && i < k_index) i++;
		memcpy(data + size * k_index, data + size * i, size);
		k_index = i;
		memcpy(data + k_index * size, key, size);
	}
	memcpy(data + k_index * size, key, size);
	quickSort(data, size, k_index, cmp_func);
	quickSort(data + (k_index + 1) * size, size, num - k_index - 1, cmp_func);
}
