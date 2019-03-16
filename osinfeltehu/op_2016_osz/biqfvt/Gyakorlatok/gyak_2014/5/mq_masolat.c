#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>


// Posix message queue demo program


int main(int argc, char **argv)
{
char mqname[] = "/alma"; // mqname must start with / !!!!! name must be unique!!!!
char rcv_buf[40];
mqd_t mqdes1, mqdes2;
pid_t pid, cpid;
int status;
//memset(&attr, 0, sizeof( attr));
struct mq_attr attr;
attr.mq_maxmsg = 5; // < /proc/sys/fs/mqueue/msg_max
attr.mq_msgsize = 40;// < /proc/sys/fs/mqueue/msgsize_max
mq_unlink(mqname);  // remove if exists

mqdes1 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);

if( (pid = fork()) < 0) 
	{
	printf("fork: Error\n");
	printf("Test FAILED\n");
	exit(-1) ;
	}
if(pid == 0) // child
	{
	// no necessary to use other mq id!
	printf("Child:\t\t\tSending message to empty queue\n");
	mq_send(mqdes1, "Hajra FTC!", 13, 30);
	printf("Child:\t\t\tFinished!\n");
    exit(0);
	}
else 
	{ /* parent */
        
	mq_receive(mqdes1, rcv_buf, 40, 0);
	printf("Parent:\t\t\tQueue transition occured - received: %s\n",rcv_buf);
  wait(NULL);
}        mq_close(mqdes1);
        mq_unlink(mqname);
        printf("\nThe mq sample is finished!\n");

        return(0);
}