#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

int getInt(char *in)
{
	// no error check if the string is not a number
	int ret = 0;
	while (*in)
	{
		ret *= 10;
		ret += *in - '0';
		in++;
	}
	return ret;
}


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s number\n", argv[0]);
		return -1;
	}

	int number = getInt(argv[1]);

	// number must be >= 1000
	int div = number / 1000;
	number /= div;

	// the result fron the division above must be an odd number
	div = number % 2;
	number /= div;

	// input string must have exactly 6 digits
	number *= argv[1][6];
	
	return number;
}
