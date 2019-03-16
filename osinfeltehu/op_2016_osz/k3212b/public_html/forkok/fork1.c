#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("Jo napot, en futok: %d\n", getpid());
    printf("innen jottem: %d\n",getppid());
    return 0;
}