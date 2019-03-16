#include <stdio.h>
#include <sys/types.h>

int main(){
 pid_t pid;
 pid=fork();
 if(pid==0){
  printf("%i \n%i \n",pid, getppid()); 
 }else{
  printf("%i \n",pid);
  pid=fork();

  if(pid==0){
  printf("%i \n%i \n",pid, getppid());
  }else{
   printf("%i \n",pid);
  }

 }
}
