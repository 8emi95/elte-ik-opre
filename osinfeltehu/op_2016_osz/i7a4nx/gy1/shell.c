#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

//az exec lecsereli a sajat munakteruleten levo kodjat a parametereben adott kodra
//a pid szam megmarad, a szulo pl tudja vizsgalni hogy befejezodik e

//beolvasasra scanf() fv, fgets(), gets()
//talan a gets() a legelavultabb
//az 1ik beolvassa az entert is, ezaltal 1el hosszabb lehet

int main()
{
 int status;
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 
 //ez ellenorzi, hogy a szuloben vagyunk e
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0); 
    printf("The end of parent process\n");
 }
 else //child process
 {
    //to start a program, like printing out a string (in parameter) 5 times (parameter)
    
//sajnos az execv kidobja a teljes munakteruletet, ezaltal a while ciklusunk is eltunik
//tehat belul kell egy fork() hivas
//de itt is veszelyes lenne vegtelen ciklusban forkolni
//fogadjon mondjuk 3 parancsot 
while (){	
	//beolvasunk 1 teljes sort es daraboljuk:
	//darabolasra strip() fuggveny?
	//parancs neve darab:
	char * cmd="./write";
	//argumentumok darab:
    char * arg[]={"./write","Operating Systems","5",NULL}; 
    printf("./write program will be called by execv function\n");
    execv(cmd,arg);
    printf("It never returns to child process (except on error)- the content of the whole child process will be changed to the new one");
}	
	
 }
 return 0;
}