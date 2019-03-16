//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

int main(int argc,char** argv){
  //int f;
	//f = open("./text.txt", O_RDONLY);
	//char c;
	//if(f < 0) {
	//	printf("This is not a valid file\n");
	//	exit(1);
	//}
	//int returnValue = 1;
	//while(returnValue != 0){
	//	returnValue = read(f, &c, sizeof(c));
	//	printf("%c", c);
	//}
	//printf("\n");
	//close(f);
	if (argc != 3 ) { printf("Number of arguments are invalid\n"); }
	FILE * f;
	f = fopen(argv[1], "rb");
	if (f == NULL) {printf("Can't open source file\n");}
	FILE * g;
	g = fopen(argv[2], "wb");
	if (g == NULL) {printf("Can't create/reach destination file\n");}
	char line[160];
	while(!feof(f)) {
		fgets(line, sizeof(line), f);
		fputs(line, g);
	}
	fclose(f);
	fclose(g);
	return 0;
}
