#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
int x = 0;
int fst = 0;
int snd = 0;

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGUSR1, handler);
  x++;
}
void handlerf(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGUSR2, handlerf);
  fst++;
}
void handlers(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  signal(SIGTERM, handlers);
  snd++;
}

int main(int argc, char** argv){
 if (argc!=2){perror("You need to specify how many steps they need to take"); exit(1);}
 int nr = atoi(argv[1]);
 printf("The number: %d",nr);
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP),
                           //handler = SIG_DFL - back to default behavior
 signal(SIGUSR2,handlerf);
signal(SIGTERM,handlers);

 int st = 0;
 srand(time(NULL));
  pid_t child=fork();
  if (child>0)
  {

  pid_t child1=fork();
  if (child1>0)
  {
    //pause(); //waits till a signal arrives
    while(x<2){}
    printf("Signal arrived\n",SIGUSR1);

    sleep(3);
    int first = rand()%2;
    printf("the number:%d\n",first);
    if (first == 1)
    {
     kill(child, SIGTERM);
     kill(child1,SIGTERM);
     printf("A masodik gyerek kezd\n");
     }
     else
     {
    kill(child, SIGUSR2);
    kill(child1, SIGUSR2);
    printf("Az elso gyerek kezd\n");
    }
    int status;
   while( wait(&status)!=-1){}

    printf("Parent process ended\n");
  }
  else
  {
    printf("Waits 3 seconds, then send a SIGUSR1 %i signal\n",SIGUSR1);
   // sleep(3);
    signal(SIGUSR1, handler);
    kill(getppid(),SIGUSR1);

    while(fst==0&&snd==0){}
    if (fst!=0)
    {
    printf("The first player starts\n");
}
else
{
 printf("I, the second player, start\n");
}
    printf("Child process ended\n");
  }
  }
  else
  {
    printf("Waits 3 seconds, then send a SIGUSER1 %i signal\n",SIGUSR1);
   // sleep(3);
    kill(getppid(),SIGUSR1);

    while(fst==0&&snd==0){}
    if(fst!=0)
    {
    printf("I, the first player, start\n");
}
else
{
printf("The second plazer start\n");
}
    printf("Child process ended\n");
  }
  return 0;
}
