#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int MyPipeFiel[2];
	char buffer[300];

	int MyPipe = pipe(MyPipeFiel);
	if (MyPipe < 0)
	{
		perror("pipe(MyPipeFiel) error!");
		exit(-1);
	}


	pid_t child = fork();
	if (child < 0) 
	{
		perror ("fork() error!");
		exit(-1);
	}

	
if(child != 0)
{

	pid_t child2 = fork();
	if(child2 < 0){
		perror("fork() error");
		exit(-1);
	}
	
	/* parent process */
	if (0 != child2)	
	{	
		read(MyPipeFiel[0], buffer, 300);
		close(MyPipeFiel[0]);
		printf("taxi service: destination is %s\n", buffer);
		wait(NULL);
	}
	/* child passanger  process */
	else
	{		printf("passanger: logged in\n");
			write(MyPipeFiel[1], argv[1], strlen(argv[1])+1);
			close(MyPipeFiel[1]);
			kill(getppid(),SIGCONT);
	}
	/*child taxi  process*/
}else{
	sleep(1);
	
}
	return 0;
}
