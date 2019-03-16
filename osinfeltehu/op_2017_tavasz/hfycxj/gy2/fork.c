#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0); 
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
	
	pid_t azonosito = getpid();
	printf("szulo PID : %i\n", azonosito);
	
	//3 folyamatot futtató
	pid_t masodik = fork();
	if(masodik == 0)
	{
		pid_t masodik_azonosito = getpid();
		printf("gyerek1 PID : %i\n", masodik_azonosito);
		
		pid_t masodik_szulo = getppid();
		printf("gyerek1 szuloje PID : %i\n", masodik_szulo);
	}
 
 }
 else //child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);
 }
 return 0;
}