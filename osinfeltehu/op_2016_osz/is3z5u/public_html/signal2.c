#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

struct rendezveny
{
	int azon;
	char helyszin[30];
};

struct latogato
{
	char nev[100];
	char email[100];
	int azon;
	time_t time;
};

int main(){

  int pipefd[2]; // unnamed pipe file descriptor array
  char sz[100];
  char buff[1000];
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  FILE * fPointer;
  fPointer = fopen("input.txt", "r");
  struct rendezveny rendezvenyek[100];
  int rszam=0;
  int index=0;
  char x[1024];
    	while (fscanf(fPointer, " %1023s", x) == 1) {
			switch(rszam%2){
            case 0:
                rendezvenyek[index].azon = atoi(x);
				rszam++;
                break;
            case 1:
                memcpy(rendezvenyek[index].helyszin, x, strlen(x)+1);
                rszam++;
				index++;
				break;
        	}
		}
    fPointer = fopen("latogatok.txt", "r");	
	int j=0;
	index=0;
	struct latogato latogatok[100];
		char y[1024];
    	while (fscanf(fPointer, " %1023s", y) == 1) {
			switch(j%4){
            case 0:
                memcpy(latogatok[index].nev, y, strlen(y)+1);
                j++;
                break;
            case 1:
                memcpy(latogatok[index].email, y, strlen(y)+1);
                j++;
                break;
            case 2:
                latogatok[index].azon = atoi(y);
                j++;
                break;
            case 3:
                latogatok[index].time = atoi(y);
                j++; 
				index++;		
                break;
        	}
    	}
		
  
  
  if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
  pid_t child=fork();
  if (child>0)
  { 
      printf("Uj rendezveny!\n");
	  close(pipefd[0]); //Usually we close unused read end
	  char str[1000];
		strcpy(str, "Rendezveny: ");
		char temp[30];
		sprintf(temp, "%d",rendezvenyek[1].azon);
		strcat(str,temp);
		strcat(str, ", Helyszin: ");
		strcat(str, rendezvenyek[1].helyszin);
      write(pipefd[1], &str,sizeof(str));
               //close(pipefd[1]); // Closing write descriptor 
               //printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); 	// flushes all write buffers (not necessary)
	pause(); //waits till a signal arrive 
    printf("KingofStands megkapta a visszajelzest!\n",SIGTERM);
	sprintf(str,"");
	int i;
	for(i=0; i<j/2; ++i)
	{
		if(latogatok[i].azon == rendezvenyek[0].azon)
		{
			strcat(str,latogatok[i].nev);
			strcat(str," ");
			strcat(str,latogatok[i].email);
			strcat(str," ");
			sprintf(temp, "%d",latogatok[i].time);
			strcat(str,temp);
			strcat(str,"\n");
		}
		
	}
	printf(str);
    write(pipefd[1], &str,sizeof(str));
	close(pipefd[1]); 
	printf("Szolok man a partneremnek h elkudltem a jelentkezoket!\n",SIGTERM);
    sleep(1);
    kill(getpid(),SIGTERM);
	int status;
    wait(&status);	
    printf("Parent process ended\n");

  }
  else 
  {
    close(pipefd[1]);  //Usually we close the unused write end
	read(pipefd[0],sz,sizeof(sz));
	//close(pipefd[0]);
	//close(pipefd[0]);  //Usually we close the unused write end
	printf("Partner megkapta az adatokat: %s",sz);
	printf("\n");
	printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);
    kill(getppid(),SIGTERM);
	//pause(); //waits till a signal arrive
	sleep(3);	
    printf("Irt az ocskos hogy kuldi a neveket, ki is irom oket!\n",SIGTERM);
	//open(pipefd[0]);
	read(pipefd[0],buff,sizeof(buff));
	printf("A: %s",buff);

    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}