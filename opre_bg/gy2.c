#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 
 
 
 pid_t parentid = getpid();
 
 
 
 if (getppid() == parentid) {
	fork();
 }
 
 if (getppid() == parentid) {
	fork();
 }
 
 printf("Hello\n");
 
 // VAGY fork, és a kövi fork csak akkor ha a szülőben vagyok
 
 

 return 0;
}