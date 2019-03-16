#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h> 

//csõvezetékek
/*
int main()
{
	int fd[2]; //file leíró file a csõvezetékhez, honnan-hova 
	//fd[0] az olvasás fd[1] az írás
	int f = pipe(fd);
	pid_t child = fork();
	int status;
	if (child > 0)
	{	
		close(fd[0]);
		int i = 1;
		for (i=1; i < 5; i++)
		{
			write(fd[1], &i, sizeof(int));
		}
		close(fd[1]);
		wait(&status);
	}
	else
	{
		close(fd[1]);
		int i;
		while(read(fd[0],&i, sizeof(int))) //0-át olvas a read ha nem sikeres ezért leáll-
		{
			printf("%i ", i);
		}
		close(fd[0]);
	}
	
	return 0;
}
*/

int main(int argc, char** argv)
{
	DIR* d = opendir("/proc");
	struct dirent* p;
	int proc;
	while (p = readdir(d))
	{
		if (isdigit(p->d_name[0]))
		{
			proc = atoi(p->d_name);
			printf("%d\n", proc);
			
			if (!strcmp(argv[1], p->d_name))
				chdir(p->d_name);
		}
	}

	return 0;
}
