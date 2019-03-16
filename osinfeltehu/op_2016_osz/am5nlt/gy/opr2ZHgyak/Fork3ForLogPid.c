#include <unistd.h>	//Fork
#include <stdio.h>	//Standard IO
#include <sys/types.h> //getpid
#include <stdlib.h> //exit

int main(int argc, char* argv[])
{
	int n = 3;
	pid_t childs[n];
	pid_t parent = getpid();
    printf( "[Szülő] pid: %d\n",  parent);
	
	int i;
    for ( i = 0; i < n; i++ )
	{
		pid_t pid = fork();
		if(getpid() == parent) childs[i] = pid;
        if ( pid == 0 )
        {
            printf( "[gyerek] pid: %d szülő: %d\n", getpid(), getppid() );
			//char c = getchar();
            exit( 0 );
        }
	}
    for ( i = 0; i < n; i++ )
	{
		wait( NULL );
		printf("[Szülő] %i-dik gyerekem: %d\n", i, childs[i]);
	}
	printf("[Szülő] leáll\n");
}

