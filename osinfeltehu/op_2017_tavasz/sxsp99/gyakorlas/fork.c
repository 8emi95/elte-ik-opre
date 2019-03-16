#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>

#define   MAX_COUNT  3
#define   BUF_SIZE   100

 int status;

void  main(void)
{
     pid_t  pid;
     int    i;
     char   buf[BUF_SIZE];
	pid = fork();
	if (pid == 0){
		sprintf(buf, "This line is from child\n");
		write(1, buf, strlen(buf));
	}else{
		waitpid(pid, &status, 0);
		sprintf(buf, "This line is from parent\n");
		write(1, buf, strlen(buf));
	}


}