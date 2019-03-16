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
#define VALUE 1234

// Posix message queue demo program
int val, code;

void handler(int signo, siginfo_t *info, void *ignored)
{
	psignal(signo, "Signal handler:\t\t"); // writes text, and : signo name
	val = info->si_value.sival_int; // info comes from notify sigev_value 
	code = info->si_code; // This is SI_MESGQ
	return;
}

int main(int argc, char **argv)
{
	struct sigaction act;
	struct sigevent notify;
	struct mq_attr attr;
	sigset_t set;
	char *mqname = "/almafa"; // mqname must start with / !!!!!
	char rcv_buf[MSGSIZE];
	mqd_t mqdes1, mqdes2;
	pid_t pid, cpid;
	int status;
	
	//memset(&attr, 0, sizeof( attr));
	
	// < /proc/sys/fs/mqueue/msg_max
	attr.mq_maxmsg = MAXMSGS;
	// < /proc/sys/fs/mqueue/msgsize_max
	attr.mq_msgsize = MSGSIZE;
	
	// remove if exists
	mq_unlink(mqname);
	mqdes1 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
	sigemptyset(&set);
	
	// signal info will be transferred via sigev_value
	act.sa_flags = SA_SIGINFO;
	// every signal is accepted
	act.sa_mask = set;	
	act.sa_sigaction = handler;
	
	// SIGUSR1 could be SIGRTMAX
	sigaction(SIGUSR1, &act, 0);
	
	// normal signal, SIGEV_NONE,SIGEV_THREAD
	notify.sigev_notify = SIGEV_SIGNAL;
	// SIGRTMAX
	notify.sigev_signo = SIGUSR1; 
	// this data is also sent by notification
	notify.sigev_value.sival_int = VALUE;
	
	// SIGEV_NONE is set, no data send
	// This sigev_value.sival_int will be sent to handler siginfo_t si_value field
	// The siginfo_t si_code will be SI_MESQ, and si_signo is set to signal number
	// si_pid is the pid of sending message, si_uid the user id
	mq_notify(mqdes1, &notify); // mq notification is set, so 
	
	printf("\nMain program:\tTesting mq notification!\n\n");
	printf("Main program:\tsigev_value=%d\n",notify.sigev_value.sival_int);
	if( (pid = fork()) < 0) 
	{
		printf("fork: Error\n");
		printf("Test FAILED\n");
		exit(-1) ;
	}
	
	// child
	if(pid == 0) 
	{
		cpid = getpid() ;
		mqdes2 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
		// no necessary to use other mq id!
		printf("Child:\t\t\tSending message to empty queue\n");
		printf("Child:\t\t\tAt the same time, sending a notification to the parent!\n");
		mq_send(mqdes2, "Hajra Vasas!", MSGSIZE, 30);
		mq_close(mqdes2);
		printf("Child:\t\t\tFinished!\n");
		
		// child exit
		exit(0);
	}
	else 
	{
		/* parent */
		waitpid( cpid, &status, 0); /* keep child status from init */
		printf("Parent:\t\t\tChild process exited, so message is sent!\n");
		printf("Parent:\t\t\tWaiting for notification\n");
		// waiting for the handler, code is set
		while(code != SI_MESGQ)
		// in the handler
		sleep(1);
		printf("Parent:\t\t\tHandler is OK!\n");
		mq_receive(mqdes1, rcv_buf, MSGSIZE, 0);
		printf("Parent:\t\t\tQueue transition occured - received: %s\n",rcv_buf);
	}
	printf("Parent:\t\tsi_code=%d\n",code);
	printf("Parent:\t\tsi_value=%d\n",val);
	
	// the value of SI_MESGQ is -3
	if(code != -3 || val != VALUE)
	{
		printf("\nTest FAILED\n\n");
		return(-1);
	}
	mq_close(mqdes1);
	mq_unlink(mqname);
	printf("\nThe mq sample is finished!\n");
	return(0);
}
