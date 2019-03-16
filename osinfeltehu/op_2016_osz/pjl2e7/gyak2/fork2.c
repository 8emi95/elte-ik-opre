#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 /*
 //pid_t en = getpid();
 pid_t  child=fork(); //forks make a copy of variables
 //if(en == getpid()){
 if(child == -1){
  perror(".."); exit(1);
 }
 if(child > 0){
  printf("szulo\n");
 }else{
  printf("gyerek\n");
 }
 */
 pid_t child = fork();
 fork();
 if(child > 0){
  int status;
  waitpid(child, &status, 0);
 }
 
 printf("Hello %i %i \n", getpid(), getppid());
 fflush(stdout);
 
 
 return 0;
}