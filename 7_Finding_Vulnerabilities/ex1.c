#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char buffer[10];
	int ok = 0;

	printf("Enter pin : \n");
	gets(buffer);

	int pin = rand() % 10000;
	char pinbuffer[5];
	sprintf(pinbuffer, "%i", pin);

	if (strcmp(buffer, pinbuffer))
	{
		printf("Nope \n");
	}
	else
	{
		printf("Wow ... you guessed right, i allow you to pass \n");
		ok = 1;
	}

	if (ok)
	{
		printf("WIN \n");
	}

	return 0;
}
