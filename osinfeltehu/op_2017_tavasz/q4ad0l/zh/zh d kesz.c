#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <wait.h> 

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MEMSIZE 1024
int ar;

int szemafor_letrehozas(const char* pathname,int szemafor_ertek){
    int semid;
    key_t kulcs;
    
    kulcs=ftok(pathname,1);    
    if((semid=semget(kulcs,1,IPC_CREAT|S_IRUSR|S_IWUSR ))<0)
	perror("semget");
    // semget 2. parameter is the number of semaphores   
    if(semctl(semid,0,SETVAL,szemafor_ertek)<0)    //0= first semaphores
        perror("semctl");
       
    return semid;
}


void szemafor_muvelet(int semid, int op){
    struct sembuf muvelet;
    
    muvelet.sem_num = 0;
    muvelet.sem_op  = op; // op=1 up, op=-1 down 
    muvelet.sem_flg = 0;
    
    if(semop(semid,&muvelet,1)<0) // 1 number of sem. operations
        perror("semop");	    
}

void szemafor_torles(int semid){
      semctl(semid,0,IPC_RMID);
}


int x=0;
struct uzenet { 
     long mtype;
     char lakcim [ 80 ]; 
     pid_t telefonszam;
}; 

void handler(int signumber,siginfo_t* info,void* nonused){
  x++;
  int tavolsag=rand()%20+1;
  printf("Tavolsag: %i\n",tavolsag);
  union sigval s_value_int={tavolsag};
  sigqueue(info->si_pid,SIGUSR1,s_value_int);
}

void handler2(int signumber,siginfo_t* info,void* nonused){
  x++;
  ar=info->si_value.sival_int*300+700;
  printf("Ar: %i\n",info->si_value.sival_int*300+700);
}


int main(int argc,char** argv){
  if (argc!=2){perror("Nem adott meg lakcimet!\n");exit(1);}
  srand(time(NULL));
  int status;
  int cso[2];
  int uzenetsor;
  key_t kulcs; 
  kulcs = ftok(argv[0],1); 
  uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
  if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
  } 
  if (pipe(cso) == -1) 
    {
      perror("Hiba a pipe nyitaskor!");
      exit(EXIT_FAILURE);
    }
  char lakcim[80];

  struct sigaction sigact;
  sigact.sa_sigaction=handler;
  sigemptyset(&sigact.sa_mask); 
  sigact.sa_flags=SA_SIGINFO;
  sigaction(SIGTERM,&sigact,NULL);
  sigact.sa_sigaction=handler2;
  sigaction(SIGUSR1,&sigact,NULL);

  key_t kulcs2;
  int sh_mem_id,semid;
  kulcs2=ftok(argv[0],1);
  sh_mem_id=shmget(kulcs2,MEMSIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
  int *bevetel;
  bevetel = shmat(sh_mem_id,NULL,0);
  semid = szemafor_letrehozas(argv[0],0);

  pid_t  utas=fork();
  if (utas<0){perror("The fork calling was not succesful\n"); exit(1);} 
  if (utas>0)
  {
    pid_t taxis=fork();
    if (taxis<0){perror("The fork calling was not succesful\n"); exit(1);}
    if (taxis>0)
    {
      close(cso[1]);
      read(cso[0],&lakcim,sizeof(lakcim));
      printf("A lakcim: %s.\n",lakcim);
      struct uzenet uz;
      uz.mtype=5;
      strcpy(uz.lakcim, lakcim);
      uz.telefonszam=utas;
      status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 ); 
      if ( status < 0 ) 
          perror("msgsnd"); 
      status = msgrcv(uzenetsor, &uz, sizeof(uz) , 5, 0 );
      printf("%s\n",uz.lakcim);
      waitpid(taxis,&status,0); 
      waitpid(utas,&status,0);
      status = msgctl( uzenetsor, IPC_RMID, NULL ); 
      if ( status < 0 ) 
           perror("msgctl"); 

      close(cso[0]);
szemafor_muvelet(semid,-1);
      printf("Bevetel: %i\n",*bevetel);
 szemafor_muvelet(semid,1);
      shmdt(bevetel);
      szemafor_torles(semid);
      shmctl(sh_mem_id,IPC_RMID,NULL);
    }
    else
    {
      struct uzenet uz; 
      status = msgrcv(uzenetsor, &uz, sizeof(uz) , 5, 0 ); 
      printf("lakcim %s, telefonszam %i\n",uz.lakcim,uz.telefonszam);
      utas=uz.telefonszam;
      struct uzenet uz2;
      uz2.mtype=5;
      strcpy(uz2.lakcim, "A taxis elindult!");
      uz2.telefonszam=utas;
      status = msgsnd( uzenetsor, &uz2, sizeof(uz2) , 0 ); 
      kill(utas,SIGTERM);
      while(x==0){usleep(50);}
  *bevetel=ar;
  szemafor_muvelet(semid,1);
  shmdt(bevetel);
    }
  }
  else
  {
    shmdt(bevetel);
    close(cso[0]);
    strcpy(lakcim,argv[1]);
    write(cso[1],&lakcim,sizeof(lakcim));
    close(cso[1]);
    while(x==0){usleep(50);}

  }
  return 0;
}
