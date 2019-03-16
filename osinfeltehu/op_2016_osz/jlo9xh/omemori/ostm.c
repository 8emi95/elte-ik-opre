#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>  //fork


#include <sys/types.h> //osztott memoria
#include <sys/shm.h>
#include <sys/ipc.h>

#include <wait.h>
#include <sys/stat.h>

int main(int argc,char* argv[])
{ pid_t child;
  char szoveg[1024];
  int memoria;			//memoria terulet azonositoja
  key_t kulcs=ftok(argv[0],1); 	//egyedi kulcsot kepez
  if (kulcs<0){perror("Kulcs hiba\n");exit(1);}
  printf("%i\n",kulcs);
  memoria=shmget(kulcs, 24 ,  IPC_CREAT | 0666 );
  if (memoria==-10) {perror("lefoglalasi hiba\n"); exit(2);}
                     else child=fork();  
  if (child<0){perror("hiba");exit(0);}
  if (child>0)                  //szulo
    {
       char* cim=shmat(memoria,NULL,0);
       if (cim==NULL){printf("Nem tud csatlakozni\n");exit(2);}
       while (1)
         {
            printf(">");fflush(stdout);
            scanf("%s",szoveg);
            strcpy(cim,szoveg);         
         }
    }else
    { 				//gyerek
       char* cim=shmat(memoria,NULL,0);
       if (cim==NULL){printf("Nem tud csatlakozni\n");exit(2);}
       while (1)
         {  sleep(1);
            printf("%s",cim); fflush(stdout);
         }    
    }
    return 0;
}
                                                                                                                                    
