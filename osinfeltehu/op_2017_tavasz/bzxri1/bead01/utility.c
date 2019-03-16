#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "utility.h"

char * getlineQ(void) {
	char * line = malloc(100), *linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;
	if (line == NULL)
		return NULL;
	for (;;) {
		c = fgetc(stdin);
		if (c == EOF)
			break;
		if (--len == 0) {
			len = lenmax;
			char * linen = realloc(linep, lenmax *= 2);
			if (linen == NULL) {
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}
		if ((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}

int readIntWithBounds(int lowerBound, int upperBound)
{
	char buffer[64];
	int readInt = 0;
	int tries = 0;
	while(readInt == 0 || readInt < lowerBound || readInt > upperBound)
	{
		if(tries > 0)
			printf("Your Input is Wrong, try again pls\n>");
		
		readString(buffer, sizeof(buffer));
		readInt = atoi(buffer);
		++tries;
	}

	return readInt;
}

void readString(char * string, unsigned int maxLength)
{
	do{
		fgets(string, maxLength, stdin);
	}while (string[0] == '\n');
	
	char * lastChar = &string[strlen(string) - 1];
	
	if (*lastChar == '\n')
		*lastChar = '\0';
}