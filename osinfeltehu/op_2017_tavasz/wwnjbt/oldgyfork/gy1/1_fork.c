#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

// Gyerekfolyamatok létrehozása, fork(), wait(), waitpid(), getpid(), getppid(),
int main()
{
	int status;
	int notacommonvalue=1;
	printf("The value is %i before forking \n", notacommonvalue);

	pid_t child = fork(); //forks make a copy of variables
	if (child<0){
		perror("The fork calling was not succesful\n"); 
		exit(1);
	}
	if (child>0) //the parent process, it can see the returning value of fork - the child variable!
	{
		//waitpid(WAIT_ANY, );
		waitpid(child,&status,0); 
		//waits the end of child process PID number=child, the returning value will be in status
		//0 means, it really waits for the end of child process - the same as wait(&status)
		printf("The value is %i in parent process (remain the original) \n", notacommonvalue);
		
		pid_t azonosito = getpid();
		printf("szulo PID: %i\n", azonosito);
		
		pid_t masodik = fork();
		if(masodik == 0){
			pid_t masodik_azonosito = getpid();
			printf("masodik_azonosito PID: %i\n", masodik_azonosito);
			
			pid_t masodik_parent_pid = getppid();
			printf("m_a parent PID: %i\n", masodik_parent_pid);
		}
		
	}
	else //child process
	{
		notacommonvalue=5; //it changes the value of the copy of the variable
		printf("The value is %i in child process \n", notacommonvalue);
		
		pid_t child_azonosito = getpid();
		printf("child_azonosito PID: %i\n", child_azonosito);
	}
	return 0;
}