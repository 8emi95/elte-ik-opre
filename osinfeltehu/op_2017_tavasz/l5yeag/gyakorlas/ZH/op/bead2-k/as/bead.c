#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 512

void childP(int,int,int[]);
void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc,char *argv[])
{
    int meret = 0;
	printf("\nHany expediciot inditsunk: (help: h)\t");
	scanf("%d",&meret);
	
	pid_t pids[meret];
	int expID[meret];
	int pipes[meret][2]; 
	
	int i = 0;
	while( i < meret ){	

		pipe(pipes[i]);	//Uj adatcsovet csinalunk a processek kozti kommuniakciora
		pid_t child = fork(); //Uj forkot nyitunk
		
		if (child == 0)
		{
			childP(i,getppid(),pipes[i]);		
			return;
		}	
		pids[i] = child;
		expID[i] = i;
		i++;
	}
	
	i = 0;
	while( i < meret ){	
		char sz[100];
		read(pipes[i][0],sz,sizeof(sz));
		if ( strcmp(sz,"ok") != 0 ){
			printf("\nVáratlan hiba");
			return;
		}
		i++;
	}
	printf("\n Az osszes expedicio keszen all az indulasra");
	printf("\n Elinditjuk az megfigyeleseket\n");
	
	i = 0;
	while( i < meret ){	
		signal(SIGCONT, handler);
		kill(pids[i], SIGCONT);
		i++;
	}
	
	while( i < meret ){	
		char sz[100];
		read(pipes[i][0],sz,sizeof(sz));
		printf("\n %s \n",sz);
		i++;
	}	
	return 0;
}

void childP(int expID,int parent_pid,int pipefd[]){
	//while(1 < 2)
	//{
		signal(SIGCONT,handler);
		char szoveg[100];
		char sz[100];
		sprintf(szoveg, "%s", "ok");
		write(pipefd[1], szoveg, sizeof(szoveg));
		pause();
		printf("\nAz %d. expedico megkezdte a megfigyelest\n", (expID+1));
		sprintf(szoveg, "%s", "1 2 3");
		write(pipefd[1], szoveg, sizeof(szoveg));
		//printf("\nAz %d expedicio keszen all\n", expID);
		//pause();	//Varunk egy signalra		
		//printf("%d expedicio jelzest kapott\n",expID);

	//}
}
