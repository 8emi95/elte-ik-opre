#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //fork
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <string.h>  //strcpy
#include <sys/ipc.h> //key_t
#include <signal.h>  //kill
#include <mqueue.h>

void handler(int signal_id) {}

struct order
{
    long id;
    int year;
    char type[255];
};

int main(int argc, char **argv)
{
    pid_t pid1 = fork();
    int msgq, status;
    char *msgq_name = "/opr";
    struct mq_attr attr;
    mqd_t mq1;
    attr.mq_maxmsg = 1;    //MAXMSGS
    attr.mq_msgsize = 255; //MSGSIZE
    mq_unlink(msgq_name);  // delete if exist
    mq1 = mq_open(msgq_name, O_CREAT | O_RDWR, 0600, &attr);
    if (mq1 < 0)
    {
        printf("mq_open error: %d \n", errno);
        return 1;
    }

    signal(SIGUSR1, handler);

    if (pid1 > 0)
    {
        pid_t pid2 = fork();
        if (pid2 > 0)
        {
            puts("[Keresztapa] ready...");
            puts("[Keresztapa] Waiting for my children... ");
            pause();
            puts("[Keresztapa] Thiefs are ready! Let's play a game... ");

            char type[255] = "";
            int i = 0;
            for (i = 1; i < argc - 1; ++i)
            {
                strcat(type, argv[i]);
                strcat(type, " ");
            }

            char year[255] = "";
            strcpy(year, argv[argc - 1]);
            printf("[Keresztapa] car type: %s\n", type);
            printf("[Keresztapa] car from year: %s\n", year);

            mq_send(mq1, type, 255, 5);
            mq_send(mq1, year, 255, 5);
            puts("[Keresztapa] Order sent... ");
            pause();
            wait(NULL);
            mq_close(mq1);
            mq_unlink(msgq_name);
            puts("[Keresztapa] done... ");
        }
        else
        {
            sleep(1);
            puts("[Felderito] ready...");
            kill(getppid(), SIGUSR1);

            struct order car_order;
            char type[255] = "";
            char year[255] = "";
            mq_receive(mq1, type, 255, NULL);
            mq_receive(mq1, year, 255, NULL);
            printf("[Felderito] My PID: %d\n", pid2);
            printf("[Felderito] car type: %s\n", type);
            printf("[Felderito] car from year: %s\n", year);
            puts("[Felderito] Searching...");
            kill(getppid(), SIGUSR1);
            mq_close(mq1);
        }
    }
    else
    {
        sleep(1);
        puts("[Szallito] ready...");
        kill(getppid(), SIGUSR1);
    }

    return 0;
}