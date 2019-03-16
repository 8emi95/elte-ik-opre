#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>//fork-hoz
#include <sys/wait.h>//wait-hez
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

void notEnoughParam(int args);//hibakezelés--> ha nincsenmegadva az összes parancssori paraméter akkor kilép a programból
void notOkChildren(int ch);//maximum 3 expedíciót lehet kezdeményezni
void oneChild();//egy expedíció
void twoChildren();//két expedíció
void handler(int signumber);//jelzés küldés a gyerktől a szülőnek

int main(int argc, char ** argv) //program parancssori paraméterben kapja meg az expedíciók számát és idejét
{
 
  int children,i;
  children=atoi(argv[1]);//annyi gyerek processz lesz ahányat parancssori paraméterben kapott (a második paraméter/a futtatandó állomány neve után/ lesz a gyerekek száma)
  
  notEnoughParam(argc);
  
  switch (children)
  {
    case 1:
      oneChild();
      break;
    case 2:
      twoChildren();
      break;
    default:
      notOkChildren(children);      
  }
  
    
  return 0;
}

void notEnoughParam(int args)
{
   if(args < 3)
  {
    perror("Nem adtad meg az összes szükséges paramétert!!\n");
    exit(1);
  }

}

void notOkChildren(int ch)
{
  if (ch > 2)
  {
    perror("Ez már túl komplex folyamat makszimum 3 gyerekkel bírok el.\n");
    exit(1);
  }
  
  if (ch < 1)
  {
    perror("Legalább egy expedíciót indítani kell.\n");
    exit(1);
  }

}

void oneChild()
{
  pid_t ch,wpid;//a gyerek filyamatok-hoz és aszülőben a gyerekekre váráshoz
  int status;
  signal(SIGTERM,handler);
  
  ch=fork();
  
  if (ch < 0)
  {
    perror("A fork hívás nem sikerült.\n");
    exit(1);
  } 
  
  if(ch > 0)//szülő
  {
    pause();//Vár míg megkapja a jelzést
    printf("",SIGTERM);
    wait(&status);
    printf("Parent process ended.\n");  
  }
  
  if(ch == 0)//gyerek
  {
    printf("Vár 2 mp-t mielőtt elküldi a SIGTERM %i jelzést\n",SIGTERM);
    sleep(2);
    kill(getppid(),SIGTERM);
    printf("GYerek folyamat vége.\n");
  }
  
}

void twoChildren()
{
  int status;
  pid_t ch1,ch2;
  printf("SZülő %i PID számú.\n",getpid());
  
  signal(SIGTERM,handler);
  
  ch1=fork();
  if (ch1 == 0)
  {//gyerek
    printf("Az első gyerek folyamat %i PID számú.\n",getpid());
    printf("Vár 2 mp-t mielőtt elküldi a SIGTERM %i jelzést\n",SIGTERM);
    sleep(2);
    kill(getppid(),SIGTERM);
    printf("Gyerek folyamat vége.\n");
  }
  if (ch1 > 0)
  {//szülő
    ch2=fork();
    if(ch2 == 0)
    {//gyerek
     printf("SZülő második gyereke %i PID számú.\n",getpid());
     printf("Vár 2 mp-t mielőtt elküldi a SIGTERM %i jelzést\n",SIGTERM);
     sleep(2);
     kill(getppid(),SIGTERM);
     printf("GYerek folyamat vége.\n");
    }
    if(ch2 > 0)
    {//szülő
      pause();//Vár míg megkapja a jelzést
      printf("",SIGTERM);
      waitpid(ch2,&status,0);//második gyerekre várás
      printf("SZülő második gyereket bevarrta - PID %i.\n",getpid());
      pause();//Vár míg megkapja a jelzést
      printf("",SIGTERM);
      waitpid(ch1,&status,0);//első gyerekre várás
      printf("Szülő első gyereket bevarrta - PID %i.\n",getpid());
    }
  }    
}

void handler(int signumber)
{
  printf("%i PID - Gyerek vagyok megérkeztem.\n",getpid());
}