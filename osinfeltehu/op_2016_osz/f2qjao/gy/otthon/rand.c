#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char nev[1024];
	char szoveg[1024];
	
	printf("Neved: ");
	scanf("%s", nev);
	
	strcpy(szoveg, nev);
	
	printf("neved: \'%s\'\n", szoveg);
}