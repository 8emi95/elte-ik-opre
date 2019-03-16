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
		
		printf("Event %ld is in session! (%ld)\n", (long)getpid(), (long)getppid());
		sleep(5);
		
		printf("Event %ld is ending! (%ld)\n", (long)getpid(), (long)getppid());
		exit(0);
	  }
	}

	char action;
	do {
		action = showMainMenu();
		
		switch(action) {

		   case '1':
				
				
				break;
			
		   case '2':
				statement(s);
				break;
		
		   default:
				break;
		}
	} while (action != 'q')
	
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

char showMainMenu() {
	/* Menu */
	printf("Options\n");
	printf("1. Start event\n");
	printf("2. End event\n");
	printf("q. Quit\n\n");
	
	char c;
	do
	c = getchar();
	while (c != '1' || c != '2' || c != 'q');
	
	return c
}