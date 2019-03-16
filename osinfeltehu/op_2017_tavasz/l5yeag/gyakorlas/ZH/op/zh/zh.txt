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

volatile pid_t *pids;
int l=0;
int eltelt[2];
void handler(int signumber){
	printf("Jelzek a szölőnek\n");
}
void handler3(int signumber){
}
struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     int azon;
	 int ido;
}; 
int kuld( int uzenetsor,int ido,int azon ) 
{ 
     struct uzenet uz = { 5,azon, ido}; 

     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz)-sizeof(long) , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, sizeof(uz)-sizeof(long), 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgrcv"); 
     else
          printf( "A kapott uzenet kodja: %ld,azon: %d,ido: %d\n", uz.mtype,uz.azon, uz.ido); 
		  if(pids[0]==uz.azon)
		  {
			eltelt[0]=eltelt[0]+uz.ido;
		  }
		  else
		  {
			eltelt[1]=eltelt[1]+uz.ido;
		  }
     return 0; 
} 
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
	int uzenetsor, status; 
    key_t kulcs;
	kulcs = ftok(argv[0],1); 
    
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
         perror("msgget"); 
         return 1; 
    } 
	if(argc!=2)
	{
		printf("tul keves argumentum!\n");
		exit(2);
	}
	int lepesszam=atoi(argv[1]);
   int pipefd[2];
   if (pipe(pipefd) == -1) 
   {
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
   }
   char oszl[8]={'A','B','C','D','E','F','G','H'};
   int sor[8]={1,2,3,4,5,6,7,8};
   int GH;

   pids = mmap(0, 3*sizeof(pid_t), PROT_READ|PROT_WRITE,
              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (!pids) {
    perror("mmap failed");
    exit(1);
  }
  memset((void *)pids, 0, 2*sizeof(pid_t));

  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
  signal(SIGUSR2,handler2); //handler = SIG_DFL - back to default behavior 
  signal(SIGSTKFLT,handler2);
  signal(SIGALRM,handler3);
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
		pids[i]=pid;
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
	int testver;
	if(azon==pids[0])
	{
		testver=pids[1];
	}
	else
	{
		testver=pids[0];
	}
	srand(azon);
	printf("Gyerek vagyok, az ID-m: %d, a testvereme: %d\n",azon,testver);
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
		int it;
		for(it=0;it<lepesszam;it++)
		{
		seged=rand()%3+2;
		sleep(seged);
		printf("gondolkoztam: %d\n", seged);
		kuld(uzenetsor,seged,azon);
		seged=rand()%8;
		S=sor[seged];
		seged=rand()%8;
		O=oszl[seged];
		printf("A lepesem: %c%d-",O,S);
		write(pipefd[1], &S,sizeof(int));
		write(pipefd[1], &O,sizeof(char));
		seged=rand()%8;
		S=sor[seged];
		seged=rand()%8;
		O=oszl[seged];
		printf("%c%d [%d]\n",O,S,azon);
		write(pipefd[1], &S,sizeof(int));
		write(pipefd[1], &O,sizeof(char));
		kill(testver,SIGALRM);
		//----
		pause();
		read(pipefd[0], &S,sizeof(int));
		read(pipefd[0], &O,sizeof(char));
		printf("megkaptam: %c%d-",O,S);
		read(pipefd[0], &S,sizeof(int));
		read(pipefd[0], &O,sizeof(char));
		printf("%c%d [%d]\n",O,S,azon);
		}
		pause();
		if(pids[2]==azon)
		{
			printf("NYERTEM!![%d]\n",azon);
		}
		else
		{
			printf("vesztettem:( [%d]\n",azon);
		}
	}
	else
	{
		printf("Én vagyok a második: %d\n",azon);
		//-------
		int it;
		for(it=0;it<lepesszam;it++)
		{
		pause();

		read(pipefd[0], &S,sizeof(int));
		read(pipefd[0], &O,sizeof(char));
		printf("megkaptam: %c%d-",O,S);
		read(pipefd[0], &S,sizeof(int));
		read(pipefd[0], &O,sizeof(char));
		printf("%c%d [%d]\n",O,S,azon);
		//----
		seged=rand()%3+2;
		sleep(seged);
		printf("gondolkoztam: %d\n", seged);
		kuld(uzenetsor,seged,azon);
		seged=rand()%8;
		S=sor[seged];
		seged=rand()%8;
		O=oszl[seged];
		printf("A lepesem: %c%d-",O,S);
		write(pipefd[1], &S,sizeof(int));
		write(pipefd[1], &O,sizeof(char));
		seged=rand()%8;
		S=sor[seged];
		seged=rand()%8;
		O=oszl[seged];
		printf("%c%d [%d]\n",O,S,azon);
		write(pipefd[1], &S,sizeof(int));
		write(pipefd[1], &O,sizeof(char));
		kill(testver,SIGALRM);
		}
		pause();
		if(pids[2]==azon)
		{
			printf("NYERTEM!![%d]\n",azon);
		}
		else
		{
			printf("vesztettem:( [%d]\n",azon);
		}
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
	kill(pids[vtlen], SIGUSR2);
	kill(pids[(vtlen+1)%2],SIGSTKFLT);
	//--------
	int it;
	for(it=0;it<lepesszam;it++)
	{
		fogad(uzenetsor);
		fogad(uzenetsor);
	}
	if(eltelt[0]<eltelt[1])
	{
		pids[2]=pids[0];
		printf("Az elso jatekos nyert [szulo]\n");
	}
	else
	{
		pids[2]=pids[1];
		printf("A masodik jatekos nyert [szulo]\n");
	}
	sleep(2);
	kill(pids[0],SIGALRM);
	kill(pids[1],SIGALRM);
	wait();
	wait();
  }
  return 0;
}