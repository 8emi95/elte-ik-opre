#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int szamlalo = 0;

void handler(int signal)
{
  szamlalo++;
  //printf("%d\n",szamlalo);
}

int main()
{
  srand(time(NULL));
  signal(SIGUSR1, handler);
  int fd[2];
  pipe(fd);
 
  pid_t ellen = fork();
  if (ellen > 0)
  {
    pid_t pecset = fork();
    if (pecset > 0)
    {
      while (szamlalo != 2) {usleep(5);}
      int szavazok;
      scanf("%d", &szavazok);
      int i = 0;
      for (; i<szavazok; i++)
      {
        int id = rand() % 100000;
        printf("Szavazo krealasa.\n");
        write(fd[1], &id, sizeof(int));
      }
        close(fd[1]);
      wait(NULL);
      wait(NULL);
    } else
    {
      close(fd[1]);
      kill(getppid(), SIGUSR1);
    }
  } else
  {
    close(fd[1]);
    kill(getppid(), SIGUSR1);
    int id;
    while (read(fd[0], &id, sizeof(int)))
    {
      printf("Azonosito: %d\n", id);
    }
    printf("vege az ellen-nek\n");
  }
  return 0;
}
