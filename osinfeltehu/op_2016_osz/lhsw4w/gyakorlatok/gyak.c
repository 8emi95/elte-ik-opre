#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>

struct Name{
	int n;
	struct Name* next;
};

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

	int pipefd1[2]; 
//	int pipefd2[2];
	
    pid_t pid1;
	pid_t pid2;
	
	struct Name numbers;
	struct Name* p = &numbers;
	srand(time(NULL));
      
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);

    if (pipe(pipefd1) == -1){
            perror("Hiba az 1-es pipe nyitaskor!");
            exit(EXIT_FAILURE);
       }
		   
    pid1 = fork();	
    if (pid1 == -1){
            perror("Fork hiba");
            exit(EXIT_FAILURE);
       }

    if (pid1 == 0)
	{// child process
                			
                printf("Adatellenorzo kesz a munkara:\n ");         
			    kill(getppid(),SIGUSR1);	
				
				//wait();
				
				printf("Elso gyerek befejezte!\n");
			    kill(getppid(),SIGUSR1);						
           }    	
           else 
    {// parent process  
			pause();
				pid2 = fork();
				if (pid2 == -1){
					perror("Fork hiba");
					exit(EXIT_FAILURE);
				}	

				if (pid2 == 0){
					printf("Pecsetelo kesz a munkara:\n");         
					kill(getppid(),SIGUSR1);	
					//wait();
					printf("Masodik gyerek befejezte!\n");
					kill(getppid(),SIGUSR1);
				}
			pause();
			
				printf("Az elnok fogadja a szavazokat!\n");
				int number, random;
				printf("\nKerem adja meg hanyan jonnek el: ");
				scanf("%d", &number); 
			
				printf("Ennyien erkeznek: %d", number);
				printf("\n");
				
				
				while (number != 0)
				{
						 
						random=rand()%10; 
						printf("%d\n", random);
						p->n = random;
						p->next = malloc(sizeof(struct Name));
						printf("%d\n", p->n);
						p = p->next;
					--number;
				}
              
	       printf("Szulo befejezte!\n");	
	   }
	   exit(EXIT_SUCCESS);   
 
 
 
  return 0;
}

/*

Int pid1 = fork();
If (pid1 > 0) //a szuloben vagyunk
{
Int pid2 = fork();
}*/

/*
#include <stdio.h>
# include <sys/types.h>
//int i=0;
int main(){

int id ;
id = fork() ;
printf("id value : %d\n",id);
    if ( id == 0 )
    {
    printf ( "Child : Hello I am the child process\n");
    printf ( "Child : Child�s PID: %d\n", getpid());
    printf ( "Child : Parent�s PID: %d\n", getppid());
    }
    else
    {
    printf ( "Parent : Hello I am the parent process\n" ) ;
    printf ( "Parent : Parent�s PID: %d\n", getpid());
    printf ( "Parent : Child�s PID: %d\n", id);
    } 

}
My output is

id value : 20173
Parent : Hello I am the parent process
Parent : Parent�s PID: 20172
Parent : Child�s PID: 20173
id value : 0
Child : Hello I am the child process
Child : Child�s PID: 20173
Child : Parent�s PID: 1

*/

