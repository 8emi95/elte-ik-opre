#include <stdio.h>
#include <unistd.h>

int main()
{
	char *nev="./filetest.c";
	// fájl létezik-e?
	int e=access(nev,F_OK);
	if (!e)
		printf("A fájlnév(%s) létezik!\n",nev);
	// van-e olvasási jog
	e=access(nev,R_OK); // W_OK, X_OK a masik parameter
	if (!e)
		printf("Van olvasási jog!\n");
}
