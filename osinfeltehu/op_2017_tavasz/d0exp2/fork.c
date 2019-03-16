#include <stdlib.h>
#include <unistd.h>	//fork
#include <sys/wait.h> 	//waitpad
#include <errno.h>


int main()
{
int i;
for (i = 0; i < 4; i++)
fork();
printf("Hello %i \n", getpid(), getppid());
return 0;

}                                                           