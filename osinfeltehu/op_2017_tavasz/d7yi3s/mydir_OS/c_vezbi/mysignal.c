#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

/*void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}*/

int main(){

    pid_t pid = fork();

    if(pid <0){
    	printf("error fork\n");
    }
    if(pid==0){
    	//child process
    	printf("this is the first child created from paren %d\n", (int)getpid());
    }else{
    	//parent process
    	printf("this is the parent after fork, %d \n it will fork again", pid );
    	pid_t pid1 = fork();
    	if(pid1<0){
    		printf("error fork\n");
    	}
    	if(pid1==0){
    		//second child
    		printf("this second child has pid num what ? %d", (int) getpid() );

    	}else{
    		//parent of second child
    		printf("parent of the second child is %d\n", pid);
    	}
    }

  return 0;
}