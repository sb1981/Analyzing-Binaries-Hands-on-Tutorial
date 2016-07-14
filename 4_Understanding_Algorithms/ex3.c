#include <stdio.h>
#include <string.h>
#include <ctype.h>

void swap(char *c1, char *c2)
{
	char c = *c1;
	*c1 = *c2;
	*c2 = c;
}

int getLetterCount(char *str, char letter)
{
	int count = 0;
	
	for(int pos=0; str[pos]; pos++)
	{
		if(tolower(str[pos])==letter) count++;
	}
	return count;
}

char getMOLetter(char *str)
{
	char ret = 'a';
	int count = 0;
	for(char letter = 'a'; letter <= 'z'; letter++)
	{
		int c = getLetterCount(str, letter);
		if(c > count)
		{
			c = count;
			ret = letter;
		}
	}
	return ret;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Program usage: %s string", argv[0]);
		return -1;
	}
	
	int length = strlen(argv[1]);
	
	for(int i=0; i<length/2; i++)
	{
		swap(&(argv[1][i]), &(argv[1][length-1-i]));
	}
	
	printf("Reversed: %s\n",argv[1]);
	
	printf("Letter with the most occurrence is: %c\n", getMOLetter(argv[1]));
	return 0;
}
