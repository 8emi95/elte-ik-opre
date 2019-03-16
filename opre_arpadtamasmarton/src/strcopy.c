#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // DateTime kezeles
#include <fcntl.h> // Rendszerhivasok file
#include <unistd.h> // fork


char* IntToStr(int number)
{
	char numberStrArray[20];
	sprintf(numberStrArray, "%d", number);
	char *numberStrPointer = (char *) malloc(1 + strlen(numberStrArray));
	strncpy(numberStrPointer, numberStrArray, (int)strlen(numberStrArray));
	return numberStrPointer;
}

int main (void)
{
	int a = 5;
	char s[2];
	strcpy(s, IntToStr(a));
	s[1] = '\0';
	printf("%s", s);
	return 0;
}
