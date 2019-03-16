#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("A programnak meg kell adni a lakcimet!\n");
    exit(1);
  }
  int fid = mkfifo("taxi", S_IRUSR|S_IWUSR);
  if(fid == -1)
  {
    printf("Hiba tortent a cso letrehozasakor\n");
    exit(1);
  }
  pid_t child = fork();
  if(child < 0)
  {
    printf("Nem tudtuk a gyereket letrehozni.\n");
    exit(1);
  }
  else if(child > 0)
  {//parent
    //read() jon ide
    char lakcim[50];
    int id = open("taxi",O_RDONLY);
    read(id,lakcim,sizeof(char)*50);
    close(id);
    printf("Egy utas jelentkezett a %s-bol\n",lakcim);
    wait(NULL);
    unlink("taxi");
  }
  else
  {//gyerek
    int id = open("taxi",O_WRONLY);
    write(id,argv[1],sizeof(char)*50);
    close(id);
  }
  return 0;
}
