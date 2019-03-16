#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
//int x=0;
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //Cél: a folyamat hajtsa végre a működést, majd álljon le
  //signal(SIGTERM,SIG_DFL) a követekző sigterm jelzéskor álljon le
  // az lenne célszerű, ha a folyamat magának küldené
  //raise(SIGTERM) függvénnyel leeht megoldani
  //++x
  //handleren belül nem szép printeket használni
  //lista van arról, hogy miket ajánlott használni a handleren belül
}
int main()
{
    signal(SIGTERM,handler);
    int pipefd[2];
    pid_t child=fork();
    char sz[100];
    char out;
    if(child>0)
    {
               printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], "Hajra Fradi!",13);
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
    }
    else
    {
	       sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
	       printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",sz);
	       printf("\n");
               close(pipefd[0]); // finally we close the used read end

    }
}