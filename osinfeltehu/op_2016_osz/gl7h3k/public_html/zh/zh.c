#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct message {
    long mtype;
    char msg[1024];
};

int send( int messagerow, struct message u)
{
    int status;

    status = msgsnd( messagerow, &u, strlen(u.msg)+1,0);
    if (status < 0)
        perror("Message fault!s");
    return 0;
}

int get( int messagerow, struct message* u)
{
    int status;

    status = msgrcv(messagerow, u, 1024, 0, 0);
    return status;
}

int main(int argc, char* argv[])
{
    pid_t child;
    int messagerow, status;
    key_t key;
    const long courtnum = atoi (argv[1]);

    srand(time(NULL));
	
	int range = courtnum - 0 + 1;
	int num = rand() % range + 0;

    key = ftok(argv[0],1);
    printf("Key: %d\n",key);
    messagerow = msgget(key, 0600 | IPC_CREAT);
    if(messagerow < 0)
    {
        perror("Msgget fault!");
        return 1;
    }

    child = fork();
	
    if(child > 0)
    {
        int r = rand()%101;
        struct message u = {num, "Train started..."};
        send(messagerow,u);
        wait(NULL);
        if ( get(messagerow,&u) < 0)
            perror("Msgsnd fault");
        else
            printf("The message's code: %ld, string: %s\n", u.mtype, u.msg);

        status = msgctl(messagerow, IPC_RMID, NULL);
        if (status < 0)
            perror("Msgctl fault");
        return 0;
    } else if (child == 0)
    {
        struct message u;
        if (get(messagerow, &u) < 0)
            perror("Msgsend hiba");
        else
            printf("The message's code: %ld, string: %s\n", u.mtype, u.msg);

        int r=rand()%101;
        if(r>=90)
        {
            strcpy(u.msg, "Get towed!");
        } else
        {
            strcpy(u.msg, "I arrived at the station!");
        }
        send(messagerow, u);
    } else
    {
        perror("fork fault");
        return 1;
    }
    return 0;
}
