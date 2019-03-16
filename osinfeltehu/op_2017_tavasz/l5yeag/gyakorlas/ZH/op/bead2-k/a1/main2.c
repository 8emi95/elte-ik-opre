#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int megerkezett_expediciok = 0;
int expediciok_szama = 0;

void handler(int signumber){
    printf("A jelzés a következő számmal megérkezett: %i \n",signumber);
    megerkezett_expediciok++;
}

void handler2(int signumber)
{
    printf("A jelzés a következő számmal megérkezett: %i \n",signumber);
}

int main(int argc,char ** argv)
{
    int expediciok_szama = atoi(argv[1]);
    int i;
    //int ido = atoi(argv[2]);
	
    pid_t szulo = getpid();
    pid_t expediciok[3];

	signal(SIGTERM,handler);
    signal(SIGUSR2, handler2);   

	int status;
    pid_t pid;
    pid_t pids[3];
    
    for (i = 0; i < expediciok_szama; i++)
    {
        if(getpid() == szulo){
            pid = fork();
            pids[i] = pid;
            if (pid < 0 )
            {
                perror("Sikertelen fork hívás\n"); 
                exit(1);                
            }
        }
    }

    if (pid == 0)
    {
        sleep(2);
        printf("Gyerek vagyok PID szamom:%i\n",getpid());
        printf("%i\n", szulo);
        kill(getppid(),SIGTERM);
        printf("SIGTERM SENT\n");
        pause();
        printf("Gyerek vege\n");
        //waiting for akg          

    }    
    else
    {
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGTERM);
        sigprocmask(SIG_BLOCK, &sigset, NULL);
        int sig;
        printf("Szulo vagyok PID szamom:%i\n",getpid());
        printf("Varok a jelzesekre\n");
        printf("expediciok_szama: %i\n", expediciok_szama);
        int i;
        for(i = 0; i < expediciok_szama; ++i) {
            sigwait(&sigset, &sig);
            printf("Signal arrived\n",SIGTERM);
            megerkezett_expediciok++;
        }

        for(i = 0; i < expediciok_szama; ++i)
        {
            printf("i: %i, pid: %i\n",i, pids[i]);
            kill(pids[i],SIGUSR2);
        }   

        printf("Megerkezett az osszes expedicio - killing process\n");
        //if (expediciok_szama == megerkezett_expediciok)

    }



    return 0;
}