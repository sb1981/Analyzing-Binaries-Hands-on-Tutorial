#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkSerial(char *username, unsigned int serial)
{
	int pos = 0;
	unsigned char* s = &serial;
	while (*username)
	{
		s[pos] ^= *username;
		username++;
		pos = (pos + 1) % 4;
	}
	return serial;
}


int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s username serial\n", argv[0]);
		return -1;
	}

	char *ptr;
	unsigned int serial = strtoul(argv[2], &ptr, 10);

	if (checkSerial(argv[1], serial))
	{
		printf("Serial WRONG!\n");
	}
	else
	{
		printf("Serial OK!\n");
	}

	return 0;
}
