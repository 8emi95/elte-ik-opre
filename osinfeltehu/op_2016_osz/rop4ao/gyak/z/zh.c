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

int main(int argc,char** argv)
{
	int db = atoi(argv[1]);
	
	pid_t ellenor = fork();
	
	
	if(ellenor > 0) //elnök
	{
		printf("elnok: %i\n",db);
		
		pid_t pecsetelo = fork();
		sleep(1);
		pause();
		
		if(pecsetelo > 0) //ellenőr
		{
			
			printf("ellenor: %i\n",db);
		}else //pecsételő
		{
			printf("pecsetelo: %i\n",db);
		}
	}
}



/*pid_t ellen = fork();

if (ellen > 0)
{
	pid_t pecsetelo = fork();
	if (pecsetelo > 0)
	{
		while(szamlalo != 2)
		{;} // pause helyett
		elnok();
		wait(NULL);
		wait(NULL);
	}else
	{
		pecset();
	}

}*/