//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main(int argc,char** argv){
	// elso feladat
	FILE * f1;
	f1 = fopen("./feladat1.txt", "w+b");
	if (f1 < 0) {
		printf("Can't create/destination file");
	}
	else {
		fputs("Uri Jozsef\n", f1);
	}
	fclose(f1);
	// masodik feladat
  // harmadik feladat
	system("date");
  // negyedik feladat
  // otodik feladat
  // hatodik feladat

	return 0;
}
