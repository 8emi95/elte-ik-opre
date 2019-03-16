#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include "gyak2.h"

int main() {
	int f = open("test.txt",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	write(f, "Teleki Miklós", sizeof(char)*4);
	close(f);	
	feladat2();
	return 0;	
}

void feladat2() {
	FILE* f = fopen("feladat2.txt", "w");
	struct Something sg;
	sg.szam1 = 2;
	sg.szam2 = 3;
	fwrite(&sg, sizeof(int), sizeof(sg), f);
	fclose(f);
	struct Something read;
	FILE* g = fopen("feladat2.txt", "r");
	fread(&read, sizeof(int), sizeof(sg), g);
	printf("%i", read.szam1);
	printf("%i", read.szam2);
	fclose(g);
}