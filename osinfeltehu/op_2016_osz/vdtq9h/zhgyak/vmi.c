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

  if(argc==1)
  {
    printf("Nem adott meg versenyzo nevet parameterkent!\n");
    exit(0);
  }

  int i=0;

  struct zhmsg potvizsgasok[100];
  int potvizsgasokcount=0;

  for(i=1;i<argc;i++)
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
  int pidchild[argc];
  pidchild[i] = fork();
  if (pidchild[i]<0)
  {
    printf("Hiba a forkolaskor\n");
    exit(1);
  }
  else if(pidchild[i]==0) //gyerek kod
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
    

    close(pipefdchild[0]);
    close(pipefdchild[1]);

   

exit(0);
  }
  else
  {

    struct zhmsg msgvp;
    msgvp.sorszam=sorszam;
    strcpy(msgvp.nev,argv[i]);
    strcpy(msgvp.feladat,"assas");
    write(pipefdparent[1], &msgvp, sizeof(msgvp));
  
    int sikerulte=0;
    read(pipefdchild[0], &sikerulte, sizeof(sikerulte));
    pause();

    if(sikerulte==0)
    {
      printf("(szulo) Nem sikerult a feladatot megoldania!( %s )\n",msgvp.nev);

strcpy(potvizsgasok[potvizsgasokcount].nev,msgvp.nev);
strcpy(potvizsgasok[potvizsgasokcount].feladat,msgvp.feladat);
potvizsgasok[potvizsgasokcount].sorszam=msgvp.sorszam;

potvizsgasokcount++;

    }
    else
    {
      printf("(szulo) Sikerult a feladatot megoldania!( %s )\n",msgvp.nev);      
    }


close(pipefdparent[0]);
close(pipefdparent[1]);


                int status;
            wait(&status);


  }


//ciklus
}



  //int kibukottindex[argc];
  //int kibukottcount=0;

  for(i=0;i<potvizsgasokcount;i++)
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
  int pidchild[argc];
  pidchild[i] = fork();
  if (pidchild[i]<0)
  {
    printf("Hiba a forkolaskor\n");
    exit(1);
  }
  else if(pidchild[i]==0) //gyerek kod
  {


    struct zhmsg msgvc;
    read(pipefdparent[0], &msgvc, sizeof(msgvc));

    printf("%s nevu versenyzo \nakinek a feladata: %s \na sorszama: %d \n\n",msgvc.nev,msgvc.feladat,msgvc.sorszam);
    int sikerulte = rand()%10+1;
    if(sikerulte<9)
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
    

    close(pipefdchild[0]);
    close(pipefdchild[1]);

   

exit(0);




}
else{


    write(pipefdparent[1], &potvizsgasok[i], sizeof(potvizsgasok[i]));
  
    int sikerulte=0;
    read(pipefdchild[0], &sikerulte, sizeof(sikerulte));
    pause();

    if(sikerulte==0)
    {
      printf("(szulo) Nem sikerult a feladatot megoldania!( %s )\n",potvizsgasok[i].nev);
    }
    else
    {
      printf("(szulo) Sikerult a feladatot megoldania!( %s )\n",potvizsgasok[i].nev);      
    }


close(pipefdparent[0]);
close(pipefdparent[1]);







                int status;
            wait(&status);

}













	return 0;
}

