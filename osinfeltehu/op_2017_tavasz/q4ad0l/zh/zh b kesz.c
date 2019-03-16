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

struct uzenet { 
     long mtype;
     char lakcim [ 80 ]; 
     pid_t telefonszam;
}; 

int main(int argc,char** argv){
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
    }
    else
    {
      struct uzenet uz; 
      status = msgrcv(uzenetsor, &uz, sizeof(uz) , 5, 0 ); 
      printf("lakcim %s, telefonszam %i\n",uz.lakcim,uz.telefonszam);
      struct uzenet uz2;
      uz2.mtype=5;
      strcpy(uz2.lakcim, "A taxis elindult!");
      uz2.telefonszam=utas;
      status = msgsnd( uzenetsor, &uz2, sizeof(uz2) , 0 ); 
      //kill(
    }
  }
  else
  {
    close(cso[0]);
    if (argc!=2){perror("Nem adott meg lakcimet!\n");exit(1);}
    strcpy(lakcim,argv[1]);
    write(cso[1],&lakcim,sizeof(lakcim));
    close(cso[1]);
  }
  return 0;
}
