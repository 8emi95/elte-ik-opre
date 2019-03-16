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
 
 //pid_t  child=fork(); //forks make a copy of variables
 //ne!
 int i;
 for(i=1; i < 4; i++)
	fork(); 
//hogy csak 3-at adjon, ellenőrizni kell a pid számokat
 printf("Hello %i\n", getpid()); // kétszer fog kiírodni, a másik folyamat is kiírja
 return 0;
}