#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#define MAX_PIDS 32


void handler(int signumber){
	printf("Jelzek a szölőnek\n");
}
int l=0;

void handler2(int signumber){
	if(signumber==SIGUSR2)
	{
		l=1;
	}
	else
	{
		l=0;
	}
}
int main(int argc, char *argv[]){



   char oszl[8]={'A','B','C','D','E','F','G','H'};
   int sor[8]={1,2,3,4,5,6,7,8};

int gyerek[2];



  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
  signal(SIGUSR2,handler2); //handler = SIG_DFL - back to default behavior 
  signal(SIGSTKFLT,handler2);

  int i;
  pid_t pid;
  for(i=0;i<2;i++)
  {
	 pid=fork();
	 if(pid==0)
	 {
		break;
	 }
	 if(pid>0)
	 {
		gyerek[i]=pid;
	 }
	 if(pid<0)
	 {
		perror("Fork hiba");
		exit(1);
	 }
  }
  
  if (pid==0)
  { 
	sleep(2);
	int azon=getpid();

	srand(azon);
	printf("Gyerek vagyok, az ID-m: %d\n",azon);
	sleep(i);
	kill(getppid(), SIGUSR1);
	//----------
	pause();
	int S;
	char O;
	int seged;
	if(l==1)
	{
		printf("Én kezdek: %d\n",azon);
		//-------

	}
	else
	{
		printf("Én vagyok a második: %d\n",azon);
		//-------
	}
  }
  else 
  {
	int azon=getpid();
    printf("Szülő vagyok, az ID-m: %d\n",azon);
	//-------
	pause();
	pause();
	//---------
	srand(azon);
	int vtlen=rand()%2;
	printf("az %d. játékos kezd\n", (vtlen+1));
	
	sleep(3);
	kill(gyerek[vtlen], SIGUSR2);
	kill(gyerek[(vtlen+1)%2],SIGSTKFLT);
	
	wait();
	wait();
  }
  return 0;
}