#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>


int main()
{
  int i;
  /*int status;
  for (i=1;i<3;i++) {
    int child_pid = fork();
    if(child_pid > 0) {
       printf("I am a child process, my id: %i, id of my parent: %i\n", getpid(), getppid()); 
       waitpid(child_pid,&status,0);
    }
  }*/
  i++;
  fork();
  printf("Depth: %i",i);
  if(i>3) return 0;

  // getpid() az a getprocessid
  // a get ppid meg a get parent process id. ha 1-essel tér vissza, akkor a szülő korábban befejeződött,
  // minthogy lekértük az volna az idjét
  // printf("Hello myid:%i parent: %i \n",getpid(), getppid());
  // Ide vissza fog majd téri a controll!
  // A fork() hívás UTÁNI rész lefut a szülőben, és a gyerekben is egyaránt.
  return 0;
}