
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>

int main(int argc, char* argv[]){
  
  
  int i;
  int status;
  int nofp=atoi(argv[2]);
  

  for(i=0; i<nofp; i++){
    if (fork()){
      execv("./prog", argv);
    //system();
      exit(0);
      }
  }
  
  
  for(i=0; i<nofp; i++){
  
    wait(&status);
  
  }
  
//  printf("I am the parent\n");

  return 0;

}