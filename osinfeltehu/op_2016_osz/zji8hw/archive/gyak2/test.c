//// LINK with -lrt
////send addional data with a signal

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <time.h>

struct student
{
    char name[80];
    int year;
};

void handler(int signumber, siginfo_t *info, void *nonused)
{
    printf("Signal with number %i has arrived\n", signumber);
    switch (info->si_code)
    {
        case SI_QUEUE:
            printf("It was sent by a sigqueue, sending process (PID %i)\n", info->si_pid);
            struct student *d = (struct student *) info->si_value.sival_ptr;
            //printf("Additional value: %s, %i\n", d->name, d->year);
            printf("ptr in parent: %p\n", d);
            break;
    }
}

int main()
{
    struct student zoli = {"Zoli", 2014};

    struct sigaction sigact;
    sigact.sa_sigaction = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_SIGINFO;
    sigaction(SIGTERM, &sigact, NULL);

    pid_t child = fork();
    if (child > 0)
    {
        wait(NULL);
        printf("Parent process ended\n");
    }
    else
    {
        struct student adam = {"Barath Adam", 2014};
        //printf("child value: %s, %i\n", zoli.name, zoli.year);
        printf("ptr in child: %p\n", &zoli);
        union sigval s_value_ptr;
        s_value_ptr.sival_ptr = &adam;
        sigqueue(getppid(), SIGTERM, s_value_ptr);
        printf("Child process ended\n");
    }
}