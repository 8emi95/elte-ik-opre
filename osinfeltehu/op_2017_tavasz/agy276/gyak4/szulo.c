#include <stdio.h>
#include <sys/types.h>

int main(){
  pid_t pid;
  pid_t pid2;
  pid = fork();

  if(pid){
	pid2 = fork();
	if(pid2){
		printf("Szulo:\n- 1. gyereke: %i \n")
	}else{
		printf(2. gyerek: %i \n)
	}
  }else{
	printf(1. gyerek: %i \n 1. gyerek);
  }
}
