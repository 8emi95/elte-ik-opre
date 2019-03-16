#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 /*int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);*/
 
 pid_t  child=fork(); //forks make a copy of variables
 /*if(child > 0)
 {
	 int status;
	 waitpid(child, &status, 0); //gyerek foylamat befejezésére vár, MINDIG a szülő vára be a gyereket, status 0 ha minden rendben lefutott, különben a hibakódot kapja meg
	 //wait(&status,0); itt bármelyik gyerekfolyamatra vár
 }*/
 fork(); // 2 hatványaival nő
 printf("Hello %i %i\n", getpid(),getppid()); // kétszer fog kiírodni, a másik folyamat is kiírja; getppid() = szülő foylamat pidszáma
 fflush(stdout); //mindenképp írja ki a képernyőre
 //valamenyni kiírásra került, majd visszalép, de aztán folytatja --> a szülő befejeződik, de a gyerek még fut, ezt nem akarjuk, a szülő várja be a gyerekfolyamatok végét: waitpid()
 return 0;
}