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

int main(int argc,char** argv){
  int status;
  int cso[2];
  if (pipe(cso) == -1) 
    {
      perror("Hiba a pipe nyitaskor!");
      exit(EXIT_FAILURE);
    }
  char* lakcim;
      lakcim=(char*)malloc(80*sizeof(char));
  pid_t  utas=fork();
  if (utas<0){perror("The fork calling was not succesful\n"); exit(1);} 
  if (utas>0)
  {
    pid_t taxis=fork();
    if (taxis<0){perror("The fork calling was not succesful\n"); exit(1);}
    if (taxis>0)
    {
      close(cso[1]);
      read(cso[0],lakcim,sizeof(lakcim));
      printf("A lakcim: %s.\n",lakcim);
      waitpid(taxis,&status,0); 
      waitpid(utas,&status,0);
      close(cso[0]);
    }
    else
    {

    }
  }
  else
  {
    close(cso[0]);
    if (argc!=2){perror("Nem adott meg lakcimet!\n");exit(1);}
    strcpy(lakcim,argv[1]);
    write(cso[1],lakcim,sizeof(lakcim));
    close(cso[1]);
  }
free(lakcim);
  return 0;
}
