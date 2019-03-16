#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
// compile: cc -l rt *.c
#define MSGSIZE 40/* maximum message size */
#define MSGTYPE 10/* message type to be sent and received */
/* Use a signal value between SIGRTMIN and SIGRTMAX */
#define SIGRT1 (SIGRTMIN+1)
/* The message buffer structure */
struct my_msgbuf {
long mtype;
char mtext[MSGSIZE];
};
struct my_msgbuf msg_buffer;
/* The message queue id */
int msqid;
/* SA_SIGINFO signal handler */
void sighandler(int, siginfo_t *, void *);
/* Set after SIGRT1 signal is received */
volatile int done = 0;
pid_t parent_pid;
pid_t child_pid;
main()
{
int retval;
sigset_t set;
struct sigaction sa;
/* Save off the parent PID for the child process to use. */
parent_pid = getpid();
/* Create a private message queue. */
msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
if (msqid == -1) 
{
	perror("msgget");
	exit(-1);
}
/* Create a child process. */
child_pid = fork();
if (child_pid == (pid_t)-1) 
{
	/* The fork(2) call returned an error. */
	perror("fork");
	/* Remove the message queue. */
	(void) msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
	exit(-1);
}
if (child_pid == 0) {
/* Child process */
/* Set the message type. */
msg_buffer.mtype = MSGTYPE;
/* Perform some work for parent. */
sleep(1);
/* ... */
/* Copy a message into the message buffer structure. */
strcpy(msg_buffer.mtext, "Hajra Fradi!");
/* Send the message to the parent using the message
* queue that was inherited at fork(2) time.
*/
retval = msgsnd(msqid, (const void *)&msg_buffer,
strlen(msg_buffer.mtext) + 1, 0);
if (retval) 
{
	perror("msgsnd(child)");
	/* Remove the message queue. */
	(void) msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
	exit(-1);
}
/* Send the parent a SIGRT signal. */
retval = kill(parent_pid, SIGUSR1);
if (retval) {
perror("kill SIGNAL");
/* Remove the message queue. */
(void) msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
exit(-1);
}
exit(0);
}
/* Parent */
/* Setup to catch the SIGRT signal. The child process
* will send a SIGRT signal to the parent after sending
* the parent the message.
*/
sigemptyset(&set);
sa.sa_mask = set;
sa.sa_sigaction = sighandler;
sa.sa_flags = SA_SIGINFO;
sigaction(SIGUSR1, &sa, NULL); // SIGRT' volt
/* Do not attempt to receive a message from the child
* process until the SIGRT signal arrives. Perform parent
* workload while waiting for results.
*/
while (!done) {
/* ... */
}
/* Remove the message queue.
(void) msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
*/
/* All done.
*/
exit(0);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
/*
* This routine reacts to a SIGRT1 user-selected notification
* signal by receiving the child process’ message.
*/
void sighandler(int sig, siginfo_t *sip, void *arg)
{
int retval;
struct ucontext *ucp = (struct ucontext *)arg;
/* Check that the sender of this signal was the child process.*/
if (sip->si_pid != child_pid) 
	{
	/* Ignore SIGRT from other processes.*/
	printf("ERROR: signal received from pid %d\n", sip->si_pid);
	return;
	}
/* Read the message that was sent to us.
*/
retval = msgrcv(msqid, (void*)&msg_buffer,
MSGSIZE, MSGTYPE, IPC_NOWAIT);
done++;
if (retval == -1) 
	{
	perror("mq_receive (parent)");
	return;
	}
if (msg_buffer.mtype != MSGTYPE) 
	{
	printf("ERROR: unexpected message type %d received.\n",msg_buffer.mtype);
	return;
	}
printf("message type %d received: %s\n",msg_buffer.mtype, msg_buffer.mtext);
}

