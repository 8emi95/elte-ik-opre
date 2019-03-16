#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_COUNT 5

void cprocess();
void process();

int main()
{
    pid_t cpid;
    cpid = fork();
    
    if(cpid == 0)
    {
		cprocess();
    }
    else
    {
		process();
    };
    
    return 0;
};

void cprocess()
{
    int i;
    for(i = 1; i <= MAX_COUNT; i++)
    {
		printf("%d\n",i);
    };
};

void process()
{
    int i;
    for(i = 1; i <= MAX_COUNT; i++)
    {
		printf("%d\n",i);
    };
};