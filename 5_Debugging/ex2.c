#include <stdio.h>
#include "ex2_lib.h"

int main(void)
{
	int array1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int array2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	int a1Size = sizeof(array1) / sizeof(int);
	int a2Size = sizeof(array2) / sizeof(int);

	printf("Array 1: ");
	printarray(array1, a1Size);
	reverse(array1, a1Size);
	printf("\nReversed array 1: ");
	printarray(array1, a1Size);
	printf("\n\nArray 2: ");
	printarray(array2, a2Size);
	reverse(array2, a2Size);
	printf("\nReversed array 2: ");
	printarray(array2, a2Size);
	printf("\n");
	return 0;
}