#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n", signumber);
}




int main(){
  int status;
  int nummer=100;
  
  printf("the value is %i before forking \n", nummer);

  //START
  int total_expeditions=0;
  int length_observations=0;
  
  printf("What are the number of expeditions? \n");
  scanf("%d", &total_expeditions);
  printf("You entered:%d\n", total_expeditions);
  
  printf("What will be the length of rabbit observations? \n");
  scanf("%d", &length_observations);
  printf("you entered:%d\n", length_observations); 

  signal(SIGTERM, handler);

  //CREATE CHILD
  pid_t child;
  int i;
  for(i=0; i<total_expeditions; i++){
    child = fork();
    printf("%d: My PID =%d  %d\n", i, (int) getppid(),  (int) getpid());
  }
  sleep(1);
  printf("---> My PID =%d  %d\n", (int) getppid(), (int) getpid());
  
  //IF ERROR
  if (child<0){ 
    perror("the fork calling was not succesful\n");
    exit(1);
  }
  
  //PARENT PROCESS
  if(child>0){
    //waitpid(child,&status,0);
    //printf("the value is %i in parent process \n", nummer);
    pause();
    printf("Signal arrived to parrent\n", SIGTERM);
    wait(&status);
    printf("parent process ended\n");
  }
  
  //CHILD PROCESS
  else{
   // nummer=5;
   // printf("the value is %i in child process \n", nummer);
  printf("wait 3 seconds, then send a SIGTERM %i signal, from child %d\n",SIGTERM, getpid());
  sleep(3);
  kill(getppid(), SIGTERM);
  printf("child process ended\n");  
  
  }
  
  return 0;
  
  

}