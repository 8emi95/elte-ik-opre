#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int main()
{
	
	pid_t pids[10];
	int i;
	int n = 10;

	/* Start children. */
	for (i = 0; i < n; ++i) {
	  if ((pids[i] = fork()) < 0) {
		perror("fork");
		abort();
	  } else if (pids[i] == 0) {
		
		printf("I'm child: %ld. My parentis: %ld\n", (long)getpid(), (long)getppid());
		sleep(1);
		
		exit(0);
	  }
	}

	/* Wait for children to exit. */
	int status;
	pid_t pid;
	while (n > 0) {
	  pid = wait(&status);
	  printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
	  --n;  // TODO(pts): Remove pid from the pids array.
	}
	
	printf("It's finaly over!\n");
	return 0;
}