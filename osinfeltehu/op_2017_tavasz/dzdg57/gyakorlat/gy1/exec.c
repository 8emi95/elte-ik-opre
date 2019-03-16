#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
 int status;
 char *argv[3];
	argv[0] = "/bin/ls";
	argv[1] = "-la";
	argv[2] = NULL;
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
    waitpid(child,&status,0); 
    printf("The end of parent process\n");
 }
 else //child process
 {
    //to start a program, like printing out a string (in parameter) 5 times (parameter)
	// ez igy gagyi... nem tudom mit kéne csinálnia...:
    //char * cmd="./write"; 
    //char * arg[]={"./write","Operating Systems","5",NULL}; 
	
	//lecseréltem valami értelmesre:
	
    printf("./write program will be called by execv function\n");
	
	// lecserélem
    // execv(cmd,arg);
	
	execv(argv[0], argv);
	
	// ez a gyerekfolyamat lecserélődik... a lecserélt folyamat ugyanazzal a pid-val rendelkezik... amikor ez a lecserélt folyamat terminál, akkor visszatér a szülőbe... (a szülő várakozott)
    printf("It never returns to child process (except on error)- the content of the whole child process will be changed to the new one");
 }
 return 0;
}