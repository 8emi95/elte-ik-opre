
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(){

  int status;
  
  printf("hello \n");
  
//  pid_t child=fork();
//  pid_t child2=fork();
  
//  if(child==0){
  //  printf("I am the child.\n");
   // }else{
   //   printf("I am the parent.\n");
   // }
  
  
    if(fork()){
      if(fork())
        ;
      else printf("I am child 2!\n");  
    }else printf("I am child 1!\n");
    
    wait(&status);
  
 // if(child==0)  
  //  printf("Child is terminating.\n");
 // else 
  //  printf("Parent is terminating.\n");  
        
    return 0;

}