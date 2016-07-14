#include <stdio.h>

void printarray(int *in, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf("%i ", in[i]);
	}
}

void reverse(int *in, int length)
{
	int pos1 = 0;
	int pos2 = length - 1;
	int tmp;

	while (pos1 != pos2)
	{
		tmp = in[pos1];
		in[pos1] = in[pos2];
		in[pos2] = tmp;
		pos1++;
		pos2--;
	}
}