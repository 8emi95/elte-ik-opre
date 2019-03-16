#include <unistd.h>
#include <sys/types.h> //pid-ekhez kell
#include <errno.h>
#include <stdio.h>
#include <wait.h>	//varakozáshoz kell
#include <stdlib.h>

int main()
{
    pid_t cpid;	// uj szal azonositoja
    int retval;	//visszateresi ertek
    int status;	//varakozo eljaras statusza
    
    cpid = fork();	//innen mar csak a gyerek vagy a szulo fog futni
    if(cpid >= 0)
    {
		if(cpid == 0)
		{
			printf("I'm the child\n");
			printf("Child's ID: %d\t",getpid());
			printf("I'm sleeping...\n");
			sleep(5);	//wait for 5 secs
			printf("Kerem a visszateresi erteket: ");
			scanf("%d",&retval);	//ertek bekerese
			exit(retval);
		}
		else
		{
			printf("I'm the parent!\n");
			printf("Parent's ID: %d\t",getpid());
			printf("The other ID: %d\t",getppid());
			printf("Bye parent!\n");
		}
    }
    else
    {
		perror("fork\n");
		exit(0);
    };
	
    return 0;
}

