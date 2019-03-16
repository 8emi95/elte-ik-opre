#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int megerkezett_expediciok = 0;

void handler(int signumber)
{
    printf("A jelzés a következő számmal megérkezett: %i \n",signumber);
    megerkezett_expediciok++;
}

void handler2(int signumber)
{
    printf("H2: A jelzés a következő számmal megérkezett: %i \n",signumber);
}


int main(int argc, char *argv[])
{
    int expediciok_szama = atoi(argv[1]);

    signal(SIGTERM,handler);
    signal(SIGUSR2,handler2);

    pid_t szulo = getpid();
    pid_t expediciok[3];

    pid_t pid;
    
    int i;
    for (i = 0; i < expediciok_szama; i++)
    {
        if(getpid() == szulo){
            pid = fork();
            expediciok[i] = pid;
            if (pid < 0 )
            {
                perror("Sikertelen fork hívás\n"); 
                exit(1);                
            }
        }
    }

    if (pid == 0)
    {
        kill(getppid(),SIGTERM);
        pause(); //wait for signal's arrival
        printf("AGYFASZ AGYFASZ BYEBYE\n");
    }
    else
    {
        while(megerkezett_expediciok != expediciok_szama) {;}
        printf("Minden expedicio bejelentkezett\n");

        //all the rabbits gonna die
        int i;
        for(i = 0; i < expediciok_szama; ++i)
        {
            printf("i: %i, pid: %i\n",i, expediciok[i]);
            kill(expediciok[i],SIGUSR2);
        }   
    }

	return 0;
}