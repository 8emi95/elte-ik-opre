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
#include <sched.h>
#include <signal.h>
#include <bits/siginfo.h>

#define MSGSIZE 40
#define MAXMSGS 5
#define VAL 1234

// Posix message queue demo program

int val, code;
//
void handlr(int signo, siginfo_t *info, void *ignored)
{
psignal(signo, "handlr:\t\t\t");
val = info->si_value.sival_int;
code = info->si_code;
return;
}

int main(int argc, char **argv)
{
struct sigaction act;
struct sigevent notify;
struct mq_attr attr;
sigset_t set;
char *mqname = "/mq_notify_rtsig";
char rcv_buf[MSGSIZE];
mqd_t mqdes1, mqdes2;
pid_t pid, cpid;
int status;
//memset(&attr, 0, sizeof( attr));
attr.mq_maxmsg = MAXMSGS;
attr.mq_msgsize = MSGSIZE;
mq_unlink(mqname);
mqdes1 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
sigemptyset(&set);
act.sa_flags = SA_SIGINFO;
act.sa_mask = set;
act.sa_sigaction = handlr;
sigaction(SIGUSR1, &act, 0); // SIGRTMAX
notify.sigev_notify = SIGEV_SIGNAL;
notify.sigev_signo = SIGUSR1; // SIGRTMAX
notify.sigev_value.sival_int = VAL;
mq_notify(mqdes1, &notify);
printf("\nmq_notify_rtsig:\tTesting notification sigev_value\n\n");
printf("mq_notify_rtsig:\tsigev_value=%d\n",notify.sigev_value.sival_int);
if( (pid = fork()) < 0) 
	{
	printf("fork: Error\n");
	printf("mq_notify_rtsig: Test FAILED\n");
	exit(-1) ;
	}
if(pid == 0) 
	{ /* child */
	cpid = getpid() ;
//	mqdes2 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
	printf("child:\t\t\tsending message to empty queue\n");
	mq_send(mqdes1, "child-test-message", MSGSIZE, 30);
//	mq_close(mqdes2);
	exit(0);
	}
else 
	{ /* parent */
	waitpid( cpid, &status, 0); /* keep child status from init */
	printf("parent:\t\t\tchild process exited\n");
	printf("parent:\t\t\twaiting for notification\n");
	while(code != SI_MESGQ) // waiting for the handler
		sleep(1);
	mq_receive(mqdes1, rcv_buf, MSGSIZE, 0);
	printf("parent:\t\t\tqueue transition - received %s\n",rcv_buf);
	}
printf("mq_notify_rtsig:\tsi_code=%d\n",code);
printf("mq_notify_rtsig:\tsi_value=%d\n",val);
if(code != -3 || val != VAL) {
printf("\nmq_notify_rtsig:\tTest FAILED\n\n");
return(-1);
}
mq_close(mqdes1);
mq_unlink(mqname);
printf("\nmq_notify_rtsig:\tTest passed\n\n");
return(0);
}
