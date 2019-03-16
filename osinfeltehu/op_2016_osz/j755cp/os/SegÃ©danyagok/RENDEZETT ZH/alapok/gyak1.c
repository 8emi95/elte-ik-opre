#include <stdio.h> //printf, scanf, fgets
#include <stdlib.h>
#include <string.h> //strlen, strcpy, strcmp, 
#include <malloc.h> //malloc, calloc, realloc, free

int hossz(char*);

/* //szoveg kiírás

int main()
{
	char szoveg1[] = "Hello1";
	char szoveg2[80] = "Hello2";
	char szoveg3[80];
	strcpy(szoveg3, "Hello3");
	char* szoveg4;
	szoveg4 = (char*)malloc(80*sizeof(char)); //ennyi bájt memóriát foglal, malloc void*-ot ad vissza
	//szoveg4 = (char*)calloc(80, sizeof(char)); //ua, mint elõzõ sor
	strcpy(szoveg4, "Hello4");
	printf("%s\n", szoveg4);
	
	return 0;
}
*/
/*//beolvasás

int main()
{
	char* szoveg1 = (char*)malloc(80*sizeof(char));
	char* szoveg2 = (char*)malloc(80*sizeof(char));
			
	fgets(szoveg2, 80, stdin); //fgets beolvassa enterig + \0-t is
	printf("%s hossza:%i\n", szoveg2, strlen(szoveg2)); 
	
	scanf("%s", szoveg1); //1. param, milyen típusként olvasom be az adatot, whitespacig olvas
	printf("%s hossza:%i\n", szoveg1, strlen(szoveg1));


	printf("%i", hossz(szoveg1));
	return 0;
}
*/
/*//parancssori paraméterek
int main(int argc, char* argv[]) // = int main(int argc, char** argv)
{
	int i  = 0;
	
	for (i = 0; i < argc; i++)
	{
		if (!(strcmp(argv[i], "alma")))
		{
			//argv[i] = "antal"; //nemjó mert nincs elég memória hosszabb szóra
			argv[i] = (char*)malloc(strlen("antal")*sizeof(char));
			strcpy(argv[i], "antal");
		}
		
		printf("%s ", argv[i]);
	}
	
	printf("\n");
	return 0;
}

*/
int hossz(char* szoveg)
{
	int i = 0;
	while (szoveg[i] != 0) //0 jelzi a szöveg végét
	{
		i = i + 1;
	}
	
	return i;
}