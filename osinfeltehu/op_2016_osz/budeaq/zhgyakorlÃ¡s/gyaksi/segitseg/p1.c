#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
  int     fd[2], nbytes;
  int i=0;
  pid_t   gyid;
  char    udv[] = "Jo napot!\n";
  char    readbuffer[80];                 
  pipe(fd);
  gyid=fork();
  printf("Gyerekazonosito 1 %i index %i \n", gyid,i);
  i=i+1;
  if(gyid  == -1)
  {
     perror("fork");
     exit(1);
  };
  printf("Gyerekazonosito : %i \n",gyid);
  
  if(gyid == 0)
  {
    /* Child process closes up input side of pipe */
    close(fd[0]);
    /* Send "string" through the output side of pipe */
    write(fd[1], udv, (strlen(udv)+1));
    exit(0);
  }
  else
  {
    /* A szulo bezarja a pipet */
    close(fd[1]);
    /* Olvasas a pipebol*/
    nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
    printf("nbytes : %i \n",nbytes);printf("fd 0 : %i \n",fd[0]);
    printf("fd 1 : %i \n",fd[1]);
    printf("A fogadott szoveg: %s \n", readbuffer);
  };
  return (0);                                                                                                                                                         }
