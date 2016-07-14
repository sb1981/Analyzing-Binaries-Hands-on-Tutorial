#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkSerial(char *username, unsigned long serial)
{
	unsigned short mask1 = 0x1234;
	unsigned short mask2 = 0xABCD;

	unsigned short *ss = (unsigned short*)&serial;
	unsigned char *sc = (unsigned char*)&serial;

	ss[0] ^= mask1;
	ss[1] ^= mask2;

	while (*username)
	{
		ss[1] += *username;
		sc[0] ^= *username;
		sc[1] --;
		username++;
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
	unsigned long serial = strtoul(argv[2], &ptr, 10);

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
