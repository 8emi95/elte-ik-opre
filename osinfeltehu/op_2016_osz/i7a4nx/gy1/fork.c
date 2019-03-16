#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

//fork akt futo folyamatot tukrozi (gyerekfolyamat), processz tablaba is bekerul

//HF hogy csinalunk egy helyett 2gyereket?
//HF mi lenne hogy ha 3-4 gyereket v tobbet is tudnank csinalni
//tanacs ha a gyerekszam tekinteteben nagy szammal szeretnenk probalkozni eloszor
//ne olyan szerevreren tegyuk meg amit leterhelhet tulzottan
//szoval ne probalkozzunk 2-3-4 nel tobbel os.inf.en


// if(child>0) {printf("szulo\n");}


int main()
{
 int status;
 int notacommonvalue=1;
//mutatot keszitunk majd erteket adunk neki malloccal
//ezt akarjuk kiprobalni notacommonvalue helyen
int *x=(int*)malloc(sizeof(int));
*x=5;

// printf("The value is %i before forking \n",notacommonvalue);
 printf("The value is %i before forking \n",*x);

//letrehozza a gyerekfolyamatot 	(return: process id)
// a fork utan a 2 folyamat teljesen fuggetlen lesz, valtozok teren is
//izolacio alapelv
 pid_t  child=fork(); //forks make a copy of variables

 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 

//the parent process, it can see the returning value of fork - the child variable!
 if (child>0) 
 {
//waits the end of child process PID number=child, the returning value 
//will be in status
//waitpid (melyik gyerekfolyamatra varakozok, status, nem csak a gyerekre var)
  waitpid(child,&status,0); 

//0 means, it really waits for the end of child process - the same as wait(&status)
//  printf("The value is %i in parent process (remain the original) \n",notacommonvalue);
  printf("The value is %i in parent process (remain the original) \n",*x);

 
 }

//child process
else
 {
    *x += 5;
    notacommonvalue=5; //it changes the value of the copy of the variable
//    printf("The value is %i in child process \n",notacommonvalue);
    printf("The value is %i in child process \n",*x);

 }
 return 0;
}