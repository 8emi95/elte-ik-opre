#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>

#define address_length 800

char address[address_length];
int pipefd[2];

void handler(int signum){printf("Jelkezelés..\n");}

void  child_utas(void)
{
    printf("Utas jelentkezik\n");
    int parent=getppid();
    write(pipefd[1], address, sizeof(address));
    kill(parent,SIGUSR1);
    printf("Utas kilép\n");
}

void  parent_taxi(int pid)
{
    printf("Központ jelentkezik\n");
    pause();
    
    char address[address_length];
    read(pipefd[0],address,sizeof(address));
    printf("Utas érkezett, címe: %s\n", address);

    pid_t pid2;
    pid2=fork();
    if(pid2<0)
    {
        printf("Fork error!\n");
        exit(1);
    }
    else if(pid2==0)
    {
        printf("Taxi indul\n");
	    printf("Taxi kilép\n");
    }
    else
    {
            wait();
            return;
    }
	wait();
    printf("Központ kilép\n");
}

int main(int argc, char ** argv) 
{
    pid_t pid;

    if(argc!=2)
    {
        printf("Nincs argumentum!\n");
        exit(1);
    }
    strcpy(address,argv[1]);
    printf("Megadott cím: %s\n",address);
    
    if (pipe(pipefd) == -1) 
    {
        printf("Hiba a pipe nyitaskor!\n");
        exit(2);
    }
    signal(SIGUSR1, handler);
    pid=fork();
    if(pid<0)
    {
        printf("Fork error!\n");
        exit(3);
    }
    else if(pid==0)
    {
        child_utas();
    }
    else
    {
        parent_taxi(pid);
    }
}


