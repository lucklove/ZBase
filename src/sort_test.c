#include <stdio.h>
#include "quicksort.h"
#include "stacksort.h"

static int
cmp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

int
main(int argc, char *argv[])
{
	int for_quick_sort[] = { 1, 3, 4, 5, 6, 2, 4, 11 };
	int for_stack_sort[] = { 1, 3, 4, 5, 6, 2, 4, 11 };
	printf("before quick sort:\n");
	for(int i = 0; i < sizeof(for_quick_sort) / sizeof(int); ++i)
		printf("%d\t", for_quick_sort[i]);
	printf("\n");
	quickSort(for_quick_sort, sizeof(int), sizeof(for_quick_sort) / sizeof(int), cmp);
	printf("after quick sort:\n");
	for(int i = 0; i < sizeof(for_quick_sort) / sizeof(int); ++i)
		printf("%d\t", for_quick_sort[i]);
	printf("\n");
	printf("before stack sort:\n");
	for(int i = 0; i < sizeof(for_stack_sort) / sizeof(int); ++i)
		printf("%d\t", for_stack_sort[i]);
	printf("\n");
	stackSort(for_stack_sort, sizeof(int), sizeof(for_quick_sort) / sizeof(int), cmp);
	printf("after stack sort:\n");
	for(int i = 0; i < sizeof(for_stack_sort) / sizeof(int); ++i)
		printf("%d\t", for_stack_sort[i]);
	printf("\n");
	return 0;	
}
