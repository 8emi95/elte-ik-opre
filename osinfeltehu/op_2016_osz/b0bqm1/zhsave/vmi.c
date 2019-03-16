#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct zhmsg
{
  int sorszam;
  char nev[100];
  char feladat[100];
};


struct msgbuf
{
	long mtype;
	int osztalyzat;
};


void signal_handler(int signum)
{
        if (signum == SIGUSR1)
        {
		printf("Kaptam SIGUSR1 signal-t\n");
	}
}

int main(int argc,char *argv[])
{
  srand(time(NULL));
  printf("\n");
  int pipefdparent[2];

  if (pipe(pipefdparent) == -1) 
  {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }

  int pipefdchild[2];

  if (pipe(pipefdchild) == -1) 
  {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }  

  int sorszam=0;

  signal(SIGUSR1,signal_handler);

  int pidchild = fork();
  if (pidchild<0)
  {
    printf("Hiba a forkolaskor\n");
    exit(1);
  }
  else if(pidchild==0) //gyerek kod
  {

    struct zhmsg msgvc;
    read(pipefdparent[0], &msgvc, sizeof(msgvc));

    printf("%s nevu versenyzo \nakinek a feladata: %s \na sorszama: %d \n\n",msgvc.nev,msgvc.feladat,msgvc.sorszam);
    int sikerulte = rand()%2;
    if(sikerulte==0)
    {
      printf("(gyerek) Nem sikerult a feladatot megoldania!( %s )\n",msgvc.nev);
    }
    else
    {
      printf("(gyerek) Sikerult a feladatot megoldania!( %s )\n",msgvc.nev);      
    }

    
    write(pipefdchild[1], &sikerulte , sizeof(sikerulte ));
    sleep(2);
    kill(getppid(),SIGUSR1);
    

    
  }
  else
  {

    struct zhmsg msgvp;
    msgvp.sorszam=sorszam;
    strcpy(msgvp.nev,argv[1]);
    strcpy(msgvp.feladat,"asas");
    write(pipefdparent[1], &msgvp, sizeof(msgvp));
  
    int sikerulte=0;
    read(pipefdchild[0], &sikerulte, sizeof(sikerulte));
    pause();


    if(sikerulte==0)
    {
      printf("(szulo) Nem sikerult a feladatot megoldania!( %s )\n",msgvp.nev);
    }
    else
    {
      printf("(szulo) Sikerult a feladatot megoldania!( %s )\n",msgvp.nev);      
    }




//                int status;
//            wait(&status);

  }







	return 0;
}

