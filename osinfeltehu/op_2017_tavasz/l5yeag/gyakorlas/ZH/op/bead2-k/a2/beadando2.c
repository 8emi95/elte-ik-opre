#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> //jelzések miatt
#include <errno.h>
#include <sys/tipes.h>
#include <sys/wait.h>//wait-hez
#include <unistd.h>//fork-hoz

void notEnoughParam(int args);//hibakezelés-->ha nincs megadva az összes parancssori paraméter akkor kilép
void oneChild();
void twoChildren();
void notOkChildren(int ch);//maximum két expedíciót lehet kezdeményezni
void sigUsr1(int sig);//gyerek szülőnek küldött jelzéséhez
void sigUsr2(int sig);//szülő gyereknek küldött jelzéséhez

//változó deklaráció
pid_t child;
int status;//gyerek véget ér
int pipefd[2];//csővezeték kommunikációhoz
char sz[10]; //tömb amibe az üzenet kerül

int main(int argc,char ** argv) //program parancssori paraméterben kapja meg a gyerekek számát és az expediciók idejét
{
  int ch,i;
  ch = atoi(argv[1]);//annyi gyerek processz amennyi a parancssori paraméterben van /min 1 - max 2 ebben a progiban/
  
  notEnoughParam(argc);//ellenőrzi elég paramétert kapott-e a program
  
  switch (ch)
  {
    case 1:
      oneChild();
      break;
    case 2:
      twoChildren();
      break;
    default:
      notOkChildren(); //ha kettőnél több gyereket akarnak létrehozni azt nem hagyja    
  }

  return 0;
}

void notEnoughParam(int args)
{
  if(args < 3)
  {
    perror("Nem adtad meg az összes paramétert.\n");
    exit(1);
  }
}


void notOkChildren(int ch)
{
  if(ch>2)
  {
    perror("Túl sok gyerek,maximum kettővel bírok el.\n");
    exit(1);
  }
  if (ch <1)
  {
    perror("Legalább egy expedíciót indítani kell.\n");
    exit(1);
  }
}

void sigUsr1(int sig)
{
  printf("Megfigyelő: Megérkeztem a heylszínre.\n");
}

void sigUsr2(int sig)//szülő visszaküldi csővezetéken a megfigyelő azonosítóját amit a megfigyelő kiolvas és kiír.
{
  close(pipefd[1])//csővezeték olvasó rész lezárása
  read(pipefd[0],sz,sieof(sz));//olvasás a csőből
  close(pipefd[0]); //olvasó rész bezárása
  printf("Megfigyelő: Azonosítóm - %s", sz);  
}