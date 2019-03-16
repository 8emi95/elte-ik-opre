#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
	int status;
	int value = 2;
	int i;
	pid_t childs[9];
	int n = 9;
	i = 0;
	while(i < n && getpid() != childs[i]) {
		i++;	
	}
	if (i == n) {
		for (i=0; i < n; i++) {
			childs[i] = fork();
			if (childs[i] < 0) {
				perror("Fork");
				exit(1);	
			}
			else if (childs[i] = 0) {
				execl("", "date", NULL);
			}
		}
		while (n > 0) {
			pid_t pid = wait(&status);
			--n;
		}
		printf("Parent with id: %d, parent's id: %d\n", getpid(), getppid());
	}
	else {
			printf("Child#%d id: %d, parent's id: %d\n", i+1, getpid(), getppid());
	}
	return 0;
}
