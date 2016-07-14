#include <stdio.h>
#include <string.h>

void getPassword(char *pass)
{
	unsigned char key=42;
	
	for(int pos=0; pass[pos]; pos++)
	{
		pass[pos]^=key;
		key=(key+13)%0xFF;
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s password\n", argv[0]);
		return -1;
	}
	
	char pass[] = { 0x6d, 0x58, 0x2b, 0x35, 0x14, 0x04, 0x1a, 0xa4, 0x00 };
	getPassword(pass);
	
	if(strcmp(argv[1], pass))
	{
		printf("Wrong password\n");
	}
	else
	{
		printf("Password OK\n");
	}
}
