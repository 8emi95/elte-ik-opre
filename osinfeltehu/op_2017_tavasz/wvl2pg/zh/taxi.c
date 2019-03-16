#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void handler(int signumber)
{
}

int main(int argc, char *argv[])
{
 signal(SIGTERM,handler);
 int pipefd[2];
 int status;
 char addr[40];
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    //pause();

    pause();
    
    read(pipefd[0],addr,40);
    printf("Cim csobol kiolvasva!");


    
    printf("Utas befejezte!");
waitpid(child,&status,0);
    printf("Ez %s",addr);

//waitpid(child,&status,0);


 }
 else if(child==0) //child process
 {
	//kill(getppid(), SIGTERM);
	//printf("Utas bejelentkezett!");
	char address[40];
	strcpy(address, argv[1]);
	write(pipefd[1], address, 40);
	sleep(1);
	kill(getppid(), SIGTERM);
	printf("Cim csobe beirva!");
	
		
	 }
 return 0;
}