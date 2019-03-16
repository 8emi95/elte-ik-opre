#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

#define SIZE 1

int main()
{
	int my_Pipe[2];
	
	pipe(my_Pipe);
	pid_t pid;
	char sz[100];

	pipe(my_Pipe);
	
	write(my_Pipe_ID[1], "Ez a szöveg!",13);
	close(my_Pipe_ID[1]); // Closing write descriptor 
	
	char ssss[30];
	read(my_Pipe_ID[0], ssss, sizeof(ssss)); // reading max 100 chars
	printf("%s\n\n", ssss);
	// close(my_Pipe_ID[0]); // finally we close the used read end
	
	open(my_Pipe_ID[1]);
	write(my_Pipe_ID[1], "Ez a szöveg!!!!!",17);
	// close(my_Pipe_ID[1]); // Closing write descriptor 
	
	char dddd[40];
	read(my_Pipe_ID[0], dddd, sizeof(dddd)); // reading max 100 chars
	printf("%s\n\n", dddd);
	close(my_Pipe_ID[0]); // finally we close the used read end
	
	write(my_Pipe_ID[1], "aaaaaaaaa",10);
	close(my_Pipe_ID[1]); // Closing write descriptor 
	
	char abab[40];
	open(my_Pipe_ID[0]);
	read(my_Pipe_ID[0], abab, sizeof(abab)); // reading max 100 chars
	//strcpy(abab, "mimimiújság?");
	printf("%s\n\n", abab);
	close(my_Pipe_ID[0]); // finally we close the used read end
	
	fflush(NULL); 	// flushes all write buffers (not necessary)
	
	exit(EXIT_SUCCESS);	// force exit, not necessary
}

int main2(int argc, char *argv[])
{
	char *sz = malloc(SIZE * sizeof(char));
	
	read(0, sz, sizeof(sz));
	
	//write(1, "%s\n", sz, sizeof(sz));
	printf("sz = %s\n", sz, sizeof(sz));
	
	free(sz);
}