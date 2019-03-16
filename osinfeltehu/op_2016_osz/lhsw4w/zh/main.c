#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

	int random, rate;
	char name[50];
	int id = 1;
	int number = 5;
	char childname[50];
	int childid;
	int childnumber;
	int pipefd1[2]; 
	int pipefd2[2];
	srand(time(NULL)); 
	//random=rand()%2; 
    pid_t pid1;
      
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);

    if (pipe(pipefd1) == -1){
            perror("Hiba az 1-es pipe nyitaskor!");
            exit(EXIT_FAILURE);
       }
	if (pipe(pipefd2) == -1){
            perror("Hiba a 2-es pipe nyitaskor!");
            exit(EXIT_FAILURE);
       }
	   
		   
    pid1 = fork();	
    if (pid1 == -1){
            perror("Fork hiba");
            exit(EXIT_FAILURE);
       }

    if (pid1 == 0)
	{// child process
                	
				pause();
				read(pipefd1[0],childname,sizeof(childname));
				read(pipefd1[0],&childid,sizeof(childid));
				read(pipefd1[0],&childnumber,sizeof(childnumber));
				close(pipefd1[0]);	
				//printf("%s", childname);
                printf(" A versenyzo neve: %s", childname); printf("\n");        
                printf(" A versenyzo sorszama: %d", childid); printf("\n");        
                printf(" A versenyzo feladatszama: %d", childnumber); printf("\n");        
                        
			  
				random=rand()%2; 
				printf("%d\n", random);
			  
				if(random == 0){
					printf("A feladat megoldasa sikertelen a versenyzonel\n");
				
				}else{
					printf("A feladat megoldasa sikeres a versenyzonel\n");
				}
			  
				write(pipefd1[1], &random, sizeof(random));
				close(pipefd1[1]); 
				
				printf("A versenyzo befejezte!\n");
			    kill(getppid(),SIGUSR1);	
	}				
           else 
    {// parent process  
			
				printf("A trener elindult!\n");
				printf("\nKerem adja meg a gyerek nevet: ");
				scanf("%s", name); 
				write(pipefd1[1], name,sizeof(name)); 
				write(pipefd1[1], &id, sizeof(id));
				write(pipefd1[1], &number, sizeof(number));
				close(pipefd1[1]);
				kill(pid1, SIGUSR2);
				pause();
				
				read(pipefd1[0],&rate,sizeof(rate)); 
				
				if(rate == 0){
					printf("A feladat megoldasa sikertelen a trenernel\n");
				
				}else{
					printf("A feladat megoldasa sikeres a trenernel\n");
				}
				
              
	       printf("A trener befejezte!\n");	
	   }

  return 0;
}


