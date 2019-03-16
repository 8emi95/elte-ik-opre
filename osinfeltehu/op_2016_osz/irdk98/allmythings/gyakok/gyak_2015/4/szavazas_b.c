#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h> //waitpid
#include <errno.h> //perror 
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

struct adat{
  int azonosito;
  int ervenyes;
};

pid_t childs[2];
int fd[2];
int f,f2;
int szavazokszama;
volatile szamlalo=0;
void handler(int sn){
  szamlalo++;
;
}
void ellenorzo(){
   
  f=open("ellenor.fifo",O_WRONLY);
  if (f<0) {perror("Fifo hiba\n"); exit(EXIT_FAILURE);}

  kill(getppid(),SIGUSR1); //munka ksz vagyok 
  printf("Ellenor vagyok - kesz a munkara\n");
  int azonosito;
  struct adat a;
  close(fd[1]);
  
  while (read(fd[0],&azonosito,sizeof(int))){
    printf("%i, ",azonosito);
    a.azonosito=azonosito; a.ervenyes=((rand()%100)>20);
    printf("Ellenor: %i\n",azonosito);
    write(f,&a,sizeof(struct adat ));
    fflush(stdout);   
  }
  close(fd[0]);
  close(f);
  printf("\nEllenor vagyok csobol olvastam, irtam \n"); 
}

void pecsetelo(){
 close(fd[0]); close(fd[1]);
 f=open("ellenor.fifo",O_RDONLY);
 if (f<0) {perror("Fifo hiba\n"); exit(EXIT_FAILURE);}
 kill(getppid(),SIGUSR1); //munkara kesz
 printf("Pecsetelo vagyok - kesz a munkara\n");
 struct adat a;
 int ervenyesek=0,ervenytelenek=0;
 int i;
 
 while (read(f,&a,sizeof(struct adat))){
   printf("Pecsetelo\n");
   if (a.ervenyes) {ervenyesek++;}else {ervenytelenek++;}
 }
 printf("fifobol %i\n",ervenyesek);
 close(f);
 
 printf("\nPecsetelo vagyok, %i ervenyes, %i ervenytelen szavazat van\n",ervenyesek,ervenytelenek); 
 f2=open("jegyzokonyv.fifo",O_WRONLY);
 if (f2<0) {perror("Fifo hiba\n"); exit(EXIT_FAILURE);} 
 write(f2,&ervenyesek,sizeof(ervenyesek));
 write(f2,&ervenytelenek,sizeof(ervenyesek));
 close(f2);
}
void elnok(){
 srand(getpid());
 printf("Elnok vagyok - mindenki kesz\n");
 //mindenki kesz volt munkara, csobe a genera adatokat 
 int i,azonosito,ervenyes,ervenytelen;
 printf("Elnok vagyok...azonositokat gyartok\n");

 close(fd[0]);
 for (i=0;i<szavazokszama;i++){
   azonosito=rand()%10000;
   write(fd[1],&azonosito,sizeof(int));
   printf("%i ,\n",azonosito);
 }
 printf("Elnok vagyok, beirtam az azonositokat a csobe\n");
 close(fd[1]);
 
 f2=open("jegyzokonyv.fifo",O_RDONLY);
 if (f2<0) {perror("Fifo hiba\n"); exit(EXIT_FAILURE);} 
 read(f2,&ervenyes,sizeof(ervenyes));
 read(f2,&ervenytelen,sizeof(ervenyes));
 printf("ervenyes: %i, ervenytelen %i\n",ervenyes,ervenytelen);
 close(f2);
 
 FILE* f3=fopen("jegyzokonyv.txt","a+");
 fprintf(f3,"%i,%i\n",ervenyes,ervenytelen);
 fclose(f3);
}
int main(int argc,char** argv)
{
  signal(SIGUSR1,handler);
  szavazokszama=atoi(argv[1]);
  
  if (mkfifo("ellenor.fifo",S_IRUSR|S_IWUSR)){perror("Fifo letrehozasi hiba\n");exit(1);} 
  if (mkfifo("jegyzokonyv.fifo",S_IRUSR|S_IWUSR)){perror("Fifo letrehozasi hiba\n");exit(1);} 
  if (pipe(fd)<0){perror("Hibas a cso letrehozas\n");exit(EXIT_FAILURE);}
   
  pid_t ellen=fork();
  if (ellen>0)
  {
    pid_t pecset=fork();
    if (pecset>0)
    {
      while(szamlalo!=2){;} //2 db pause helyett
      elnok();
      wait(NULL);
      wait(NULL);
      close(f);
      unlink("ellenor.fifo");
      unlink("jegyzokonyv.fifo");
    }
    else {
      pecsetelo();
    };  
  }
  else {
    ellenorzo();
  }   
  return 0;
}
