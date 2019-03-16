#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <string.h>

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

int main(int argc,char** argv)
{
	char nev[1000];
	//nev = argv[1];
	
	char feladat[1000] = "1. FELADAT";
	int sorszam = 1;
	
	signal(SIGTERM,handler);
	
	//FIFO fájl
	int w;
	w = open("pipe.file",O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	close(w);
	
	FILE * f;
	f = fopen("pipe.file","a+");
	fputs(feladat,f);
	fputs("\n",f);
	fputs(argv[1],f);
	fputs("\n",f);
	char s[10];
	s = sprintf(s,sorszam);
	fputs(s,f);
	
	mkfifo("pipe.file", S_IRUSR | S_IWUSR );
	
	//-----------------------------------------------------------
	
	pid_t child = fork();
	
	if(child > 0)
	{
		//szülő
		pause();
		wait(NULL);
	
	}else
	{
		//gyerek
		FILE * p;
		char t[1000];
		p = fopen("pipe.file","a+");
		printf("Versenyző: megkaptam a feladatot:\n");
		while(fgets(t,sizeof(char)*1024,p))
		{
			printf("%s", t);
		}        
		fclose(p);
		unlink("pipe.file");
		kill(getppid(),SIGTERM);
	}
}