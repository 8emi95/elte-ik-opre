#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

void fv(int handle)
{
	char nev[] = "Finta Gábor";
	write(handle, &nev, sizeof(nev));
}

int main()
{
	//FILE* f = fopen("nevem", "w"); ez is jó
	//fputs("Finta Gábor\0", f);
	//close(f);
	
	int handle = open("./nevem2.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	fv(handle);
	close(handle);
	return 0;
}