#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main()
{
 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);
 
 signal(SIGTERM,handler);
 signal(SIGTERM,handler);
 
 pid_t ellenoriz=fork();
 pid_t pecsetel=fork();
 if (ellenoriz<0 || pecsetel<0){perror("The fork calling was not succesful\n"); exit(1);} 
 
 if(ellenoriz>0 && pecsetel>0){
	pause();
	printf("signal\n");
	waitpid(ellenoriz,&status,0);
	waitpid(pecsetel,&status,0);	
	printf("1\n");
} else {
	if(ellenoriz == 0 && pecsetel > 0){
		sleep(1);
		kill(getppid(),SIGTERM);
		printf("2\n");
	}
	if(pecsetel == 0 && ellenoriz > 0){
		sleep(1);
		//kill(getppid(),SIGTERM);
		printf("3\n");
	}
	if(ellenoriz == 0 && pecsetel > 0){
		printf("4\n");
	}
}

 return 0;
}