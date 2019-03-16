#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child>0){
	 printf("Szulo\n");}
	else{
		printf("Gyerek\n");
	} 
 return 0;
}