#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int done = 1;

void handler(int signumber)
{
 
}
 
void sig_handler(int signumber)
{
  done = 0;
}

int main()
{
  signal(SIGUSR1, handler);
  signal(SIGTERM, sig_handler);
  int addresspipe[2];
  int timepipe[2];
  char sz[255];
  if(pipe(addresspipe) == -1)
  {
   perror("Error opening address pipe!");
   exit(1);  
  }
  if(pipe(timepipe) == -1)
  {
   perror("Error opening time pipe!");
   exit(1);
  }
  pid_t child = fork();
  if(child == -1)
  {
    perror("Forking error");
    exit(1);
  }
  if(child > 0)
  {
   close(addresspipe[0]);
   close(timepipe[1]);
   pause();
   printf("Az idomar kesz a munkara!\n");
   FILE *f;
   FILE *g;
   f = fopen("data", "r");
   g = fopen("tmp", "w");
   if(f < 0)
   {
    printf("Error opening data file.");
    kill(child, SIGKILL);
    exit(1);
   }
   if(g < 0)
   {
    printf("Error opening work file.");
    kill(child, SIGKILL);
    exit(1);
   }
   char* line = NULL;
   size_t len = 0;
   while(getline(&line, &len, f) != -1)
   {
    if(line[strlen(line)-2] == '1')
    {
     int i;
     char* a;
     char m[255];
     a = strtok(line, ",");
     for(i = 0; i < 2; i++)
     {
      a = strtok(NULL, ", ");
     }
     snprintf(m, 255, "%s", a);
     write(addresspipe[1], m, strlen(m)+1);
     int time;
     read(timepipe[0], &time, sizeof(int));
     printf("A befogas ideje: %i\n", time);
     kill(child,SIGUSR1);
     fflush(NULL);
    }
    else
    {
     fprintf(g, "%s", line);
    }
   }
   kill(child, SIGTERM);
   wait();
   close(timepipe[0]);
   close(addresspipe[1]);
  }
  else
  {
   close(addresspipe[1]);
   close(timepipe[0]);
   int waiting;
   int readbyte;
   printf("Az idomar elkeszul a madarral...\n");
   sleep(2); //Elkeszul a teendokkel
   kill(getppid(), SIGUSR1);
   while(done)
   {     
       read(addresspipe[0], sz, sizeof(sz));
       printf("Megvan a cim: %s\n", sz);
       waiting = rand()%5+1;
       sleep(waiting);
       printf("Megvan a dron! Johet a kovetkezo!\n");
       write(timepipe[1], &waiting, sizeof(waiting));
       pause();
   }    
   close(addresspipe[0]);
   close(timepipe[1]);
  }
}
