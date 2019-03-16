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
 pid_t en = getpid();
 pid_t child=fork(); //forks make a copy of variables
 if(en == getpid())
 {
	 printf("szulo\n");	 
 }
 else{
	 printf("gyerek\n");
 }
 //ennél egyszerűbb:
 //child lehet -1 is ha nem sikerült létrehozni a gyerekfolyamatot
 if(child == -1) {perror(".."); exit(1);}
 if(child>0) //akkor biztos a szülőben vagyunk
 {
	 printf("szulo\n");	 
 }
 else
 {
	 printf("gyerek\n");
 }
 return 0;
}