#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char buffer[10];
	int ok = 0;

	if (argc != 2)
	{
		printf("usage: %s password", argv[0]);
		return -1;
	}
	snprintf(buffer, sizeof(buffer), argv[1]);

	int pin = rand() % 10000000000;
	char pinbuffer[10];
	sprintf(pinbuffer, "%i", pin);

	if (strcmp(pinbuffer, buffer))
	{
		printf(buffer);
		printf(" was incorrect \n");
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
