#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>

#define CHILDREN 2
#define MAX_SIZE 512

int main(int argc,char *argv[])
{
	char* cim = argv[0];
	int status=0;
	int pipe_file[2];
	int Xpipe = pipe(pipe_file);
	if (Xpipe < 0)
	{
		perror("pipe(pipe_file) error!");
		exit(-1);
	}

	pid_t child = fork();
	if (child < 0) 
	{
		perror ("fork() error!");
		exit(-1);
	}
	
	//parent
	if (0 != child) 
    {
		char buffer[MAX_SIZE];
		
		//waitpid(child,&status,0); 
		
		read(pipe_file[0], buffer, MAX_SIZE);
		printf("%s\n", buffer);
	}
	//utas
	else
	{
		//char buffer[MAX_SIZE];
		write(pipe_file[1], cim, sizeof(cim));
		
		exit(0);
	}
}