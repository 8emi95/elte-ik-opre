#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

struct mess
{
    long msType;
    int nr;
};
typedef struct mess Message;


int main (int argc, char* argv[])
{
    pid_t child;
    int uzenetsor, status;
    char* mqname="/baba1";

    struct mq_attr attr;
    mqd_t mq1,mq2;
    attr.mq_maxmsg=5; //MAXMSGS
    attr.mq_msgsize=50; //MSGSIZE

    mq_unlink(mqname); // delete if exist
    mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);

    if ( mq1 < 0 )
    {
        printf("mq_open error: %d \n",errno);
        return 1;
    }

    //char buffer[100];	// mq_send, mq_receive uses char array
    //printf("Buffer length: %d\n",strlen(buffer));

    child = fork();

    if ( child > 0 )
    {
        //int msg = 5;
        Message msg;
        //msg.msType=32;
        //msg.nr=10;
        //get message
        sleep(3);

        printf("Message : %d\n", msg.nr);
        int db=mq_receive(mq1,(char *) &msg, sizeof(Message), 0);
        if(db<0)
        {
        printf("Baj van a fogadassal\n");
        }
        printf("Message : %d\n", msg.nr);
        wait( NULL );
        // After terminating child process, the message queue is deleted.
        mq_close(mq1);
        mq_unlink(mqname);
        printf("Szulo vege!\n");
    }
    else
    {
        Message msg;
        msg.msType=32;
        msg.nr=18;
        printf("Message child :%d \n", msg.nr);
        //send message
        int db=mq_send(mq1,(char *)  &msg, sizeof(Message), 0);
        if(db<0)
        {
         printf("a kuldessel van baj\n");
        }
        printf("Message sent:\n");
        mq_close(mq1);
        return 0;
        // The child process receives a message.
    }

    return 0;
}