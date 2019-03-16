#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main()
{
     pid_t child = fork();
     int vmi=0;
     int status;
    if (child < 0) 
    {
        perror ("fork() error!");
        exit(-1);
    }
    if(child>0)
    {
        printf("%s","Varok");
        while(vmi==0)
        {
        }
        printf("Vege");
        waitpid(child, &status, 0); 
    }
    else if(child==0)
    {
        int i;
        for(i=0;i<5;++i)
        {
        }
        vmi++;
        printf("%s","atallitottam");
    }

}