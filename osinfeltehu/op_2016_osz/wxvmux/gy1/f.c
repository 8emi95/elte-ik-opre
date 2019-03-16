#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	int status;
 pid_t en = getpid();
 pid_t  child=fork(); 
/* if (en == getpid()){
	 // alt: child>0
	 // ez a szulofolyamat
	 printf("szulo");
 }else{
	 printf("gyerek");
 }*/
 	
 fork();
 fork();
 if (en == getpid()){
	wait(&status);
 }
 
 printf("asd %i  %i\n", getpid(), getppid());
 //getppid() szulofolyamat pid-je
 fflush(stdout); // forceolt kiíratás, mivel a pidek kiíratási sorrendje likvid
 
 return 0;
}
