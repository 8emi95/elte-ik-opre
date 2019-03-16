#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int MyPipeFiel[2];
	int MyPipeFiel2[2];
	char buffer[300];
	char buffer2[300];
	
	if(argc != 2) 
	{ printf("A program egy darab lakcimet var argumentumkent!\n"); exit(1); 
	}

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
	
	/* szülő folyamat */
	
	if (0 != child2)	
	{	
		wait(NULL);
		read(MyPipeFiel[0], buffer, 300);
		close(MyPipeFiel[0]);
		printf("Taxi service: destination is %s \n", buffer);
		printf("\n");
		write(MyPipeFiel[1], buffer, strlen(buffer)+1);
		close(MyPipeFiel[1]);
	}
	/* gyerek folyamat (utas) */
	else
	{		printf("passanger: logged in\n");
			write(MyPipeFiel[1], argv[1], strlen(argv[1])+1);
			close(MyPipeFiel[1]);
			kill(getppid(),SIGCONT);
	}
	/*gyerek folyamat (taxi)*/
}else{

	wait(NULL);
	read(MyPipeFiel[0],buffer2,strlen(buffer2));
	close(MyPipeFiel[0]);
	printf("driver received the following address: %s\n", buffer2);
}
	return 0;
}
