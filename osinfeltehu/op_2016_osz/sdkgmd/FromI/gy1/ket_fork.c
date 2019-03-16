#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 int notacommonvalue=1;
 printf("The value is %i before forking \n",notacommonvalue);
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent 
 {
	int pid=fork(); // masodik gyerek
	if (pid>0)
	{
	printf("Varunk az elso gyerekre!\n");
    waitpid(child,&status,0); 
	printf("Varunk a masodik gyerekre!\n");
	waitpid(pid,&status,0);
    printf("Mindket gyerek vegzett! \n");
	}
	else	// masodik gyerek
	{
		printf("En vagyok a masodik gyerek!\n");
	};
 
 }
 else //first child process
 {
    notacommonvalue=5; //it changes the value of the copy of the variable
    printf("The value is %i in child process \n",notacommonvalue);
	
 }
 return 0;
}
