#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main() {    
    signal(SIGTERM,SIG_IGN); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
    // handler = SIG_DFL - back to default behavior 
  
	printf("Teszt\n");
  
    pid_t child=fork();
	
	printf("After fork\n");
	
    if (child>0) // akkor szülő
    { 
	printf("Parent after fork\n");
	int i;
	for(i=0; i<100; i++) {
		printf("%i \n",i);
	}
  }
  else 
  {    
    printf("Child after fork\n"); 
		int i;
		for(i=0; i<100; i++) {
		printf("%i \n",i);
	}
  }
  
  return 0;
}