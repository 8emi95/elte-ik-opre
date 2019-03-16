#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main (void)
{
	char *szam;
	szam = (char*) calloc(1, sizeof(char));
	printf("\nKerem a szamot:\n");
	scanf("%[^\n]s", szam);
	getchar();
	
	printf("A szam: %s\nA szam elso karaktere: %c\n", szam, (char)szam[0]);
	
	char *fontos;
	fontos = (char*) calloc(1, sizeof(char));
	
	strncpy(fontos, szam + 1, strlen(szam));
	printf("A FONTOS: %d\n", atoi(fontos));
	
	switch(szam[0])
	{
		case '0':
			printf("NULLA\n");
			break;
		case '1':
			printf("EGY\n");
			break;
	}
	
	return 0;
}
