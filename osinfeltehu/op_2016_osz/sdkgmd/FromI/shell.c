#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> //strtok

int main()
{
  pid_t child;
  char  cmd[80];
  char* arg[]={"ls","-l",NULL}; // paraméterek, a második helyén lehet pl: -al
  // az arg[0] elem a parancs neve
  char line[80];
  int status;
  int argc=0;
while (1){
  int i=0;
     printf("Parancs\n> ");
     scanf("%s",cmd);
     i=strlen(cmd);
     printf("A parancs: %s hossza: %d\n",cmd,i);
     cmd[i]=0;
     if (!strcmp("vege",cmd))
	exit(1);
     child=fork();
     if (child==0)
    	    { //gyerek
	    //execlp("ls","ls","-l",(char*)NULL); // Az execl használata
    	    printf("Vegrehajtas indul\n");
    	    // execv("/bin/ls",arg); // az elso parameter teljes utvonalat var
    	    arg[0]=NULL;
	    execvp(cmd,arg); //cmd helyett: "ls" 
    	    printf("Hiba a vegrehajtasban!\n"); // ha visszatér execvp, hiba van
    	    }
         else
    	    {   //szulo
    	    waitpid(child,&status,0);
    	    printf("A szulo befejezodott, a statusz erteke: %i\n", status);
    	    }     
 }
}
