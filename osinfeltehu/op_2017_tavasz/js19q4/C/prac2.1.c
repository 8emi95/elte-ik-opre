#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close


int main(int argc, char ** argv)
{
	//take arguments
	char * source_f_n;
	char * n_of_copy;
	int f,g;
	//argc =  number of argiments passed to program
	//argv =  argument vector (one dimensial array of string) 
	if(argc > 3){
		perror("The number of arguments has to be 2");
		return 1;
	}
	else if(argc == 3){
		source_f_n = argv[1];
		n_of_copy = argv[2];
		printf("%s is source file \n%s is copy of file\n",source_f_n, n_of_copy );
	}

	// open files
	f = open(source_f_n, O_RDONLY|O_EXCL);
	if(f < 0){
		perror("Error at opening source file\n");
		exit(1);
	}

	g = open(n_of_copy, O_WRONLY|O_TRUNC|O_APPEND); 
	if(g < 0){
		perror("Error at opening copy file\n");
		exit(1);
	}

	char c;
	while (read(f, &c, sizeof(c)) ){
		printf("%c\n", c);

		if(write(g, &c, sizeof(c) != sizeof(c)) ){

			perror("There is mistake in writing\n");
			exit(1);
		}
	}
	//write(g, &c, sizeof(c));
	printf("It is successfully written to copy file\n");



	close(f);
	close(g);
    





}