#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void pecset()
{  
  kill(getppid(),SIGNAL1);
  printf("Pecsetelo vagyok!");
}

void ellenor()
{
  kill(getppid(),SIGNAL1);
  printf("Ellenor vagyok!");
}

void elnok()
{
  printf("Elnok vagyok!");
  int i,azonosito;
  printf("Elnok azonositot csinalt!");
  close(fd[0]);
  for (i=0;i<szavazokszama;i++)
  {
    azonosito=rand()%10000;
    write(f[1],&azonosito,sizeof(int));
    printf("%i ,\n",azonosito);
  }
  close(fd[1]);
}

int main(int argc)
{
  sinal(SIGUSR1,handler);

  pid_t ellen=fork();
  if(ellen>0)
  {
   
   pid_t pecsetelo=fork();
   if(pecsetelo>0)
   {
     while(szamlalo!=2){;}
     elnok();
     wait(NULL); wait(NULL);
   } 
   else
   {
     pecset();
   }
  }
  else
  {
    ellenorzo();
  }
}