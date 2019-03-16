#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

int main(int argc, char * argv[]){
  char cmd[80]="";
  pid_t child;
  int status;
  int i;
  FILE* f;
  int PID_n;
  printf("Kezdem\n");
  child=fork();
  if (child==0){ //gyerek
  for (i=1;i<argc;i++)
  {
    strcpy(cmd,"");
    strcat(cmd,"ps | grep ");
    strcat(cmd,argv[i]);
    strcat(cmd," | cut -f1 -d' '  >> pid.txt") ; 
    system(cmd);	//programb�l v�grahajt egy m�sikat
	//system v�grehajt
	//signal: figyel
	//kill: lel�
  }
  }
  else
  {
    wait(&status);    
    printf("Most olvas...\n");
    f=fopen("pid.txt","r");
    while (fgets(cmd,80,f))
    {
       PID_n=atoi(cmd);
       printf("%i\n",PID_n);
       kill(PID_n,SIGKILL); //leallitja a folyamatokat
	   //PID_n: az ilyen azonos�t�n fut� folyamatokat lel�vi
    };
    fclose(f);
    //system("rm pid.txt");
    printf("Vege\n");
    };
    return 0;
}
    