#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#define MAXLEN 2048

char *lakcim;
int ppipe[2];

int main(int arg, char *args[])
{
	if(arg > 1 && arg < 3){
		if(args[1] != NULL)
		{
			lakcim = (char *)malloc(sizeof(char)*strlen(args[1]));
			strcpy(lakcim, args[1]);
			printf("Check lakcim: %s\n", lakcim);
		}
	}else{
		printf("Hibás argumentum.\n");
		exit(1);
	}

	pid_t utas;
	pipe(ppipe);
	utas = fork();
	if(utas < 0){
		perror("fork");
		exit(1);
	}else if(utas == 0){
		close(ppipe[0]);		
		write(ppipe[1], &lakcim, MAXLEN);
		close(ppipe[1]);
	}else{	
		//while(1){
		  printf("\n");
		  wait(&utas);
		  close(ppipe[1]);
		  char *parent_cim = (char *)realloc(NULL, sizeof(char)*MAXLEN);
		  read(ppipe[0], &parent_cim, MAXLEN);
		  close(ppipe[0]);
		  printf("A kapott cím: %s\n", parent_cim);
		//}
	}

		
	return 0;
}
