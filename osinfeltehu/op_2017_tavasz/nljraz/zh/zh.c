#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <wait.h>

// Boolean
#define bool               int
#define true               1
#define false              0
#define BoolToStr(boolean) ((boolean) == true ? "true" : "false")

// Memory
#define New(type)                    ((type*)(malloc(sizeof(type))))
#define NewArray(type, count)        ((type*)(malloc((count) * sizeof(type))))
#define Delete(ptr)                  free(ptr)
#define ResizeArray(type, ptr, size) ((type*)(realloc(ptr, (size) * sizeof(type))))

// Process
#define TProcID           pid_t
#define GetParentProcID() getppid()
#define GetProcID()       getpid()
#define CreateProcess()   fork()
#define IsSucceeded(pid)  ((pid) < 0 ? false : true)
#define IsChild(pid)      ((pid) == 0 ? true : false)
#define IsParent(pid)     ((pid) > 0 ? true : false)
#define WaitForChildren() wait(NULL)
#define Sleep(tick)       sleep(tick)

// Signal
#define TSigSet                        sigset_t
#define ClearSigSet(sigset)            sigemptyset(&(sigset))
#define FillSigSet(sigset)             sigfillset(&(sigset))
#define AddToSigSet(sigset, sig)       sigaddset(&(sigset), sig) /* SIGTERM SIGUSR1 SIGUSR2   SIGKILL SIGSTOP */
#define RemoveFromSigSet(sigset, sig)  sigdelset(&(sigset), sig)
#define SetSignalMasking(sigset, mode) sigprocmask(mode, &(sigset), NULL) /* SIG_BLOCK SIG_UNBLOCK SIG_SETMASK */
#define TSigHndl                       struct sigaction
#define InitSigHndl(sighndl, handler)  { sighndl.sa_handler = (void(*)(int))(handler); sigemptyset(&sighndl.sa_mask); sighndl.sa_flags = 0; } /* SIG_DFL SIG_IGN */
#define SetSigHndl(sighndl, sig)       sigaction(sig, &sighndl, NULL);
#define Suspend(sig)                   { sigset_t __sigset__; sigfillset(&__sigset__); sigdelset(&__sigset__, sig); sigsuspend(&__sigset__); }
#define SendSignal(pid, sig)           kill(pid, sig)

// Timer
#define TTimer                               struct itimerval
#define SetSigHndlForTimer(sighndl, handler) { sigemptyset(&sighndl.sa_mask); sighndl.sa_handler = handler; sighndl.sa_flags = 0; sigaction(SIGALRM, &sighndl, NULL); }
#define InitTimer(timer, interval)           { timer.it_interval.tv_sec = 0; timer.it_interval.tv_usec = interval * 1000; timer.it_value.tv_sec = 0; timer.it_value.tv_usec = timer.it_interval.tv_usec; }
#define StartTimer(timer)                    setitimer(ITIMER_REAL, &timer, NULL)
#define SetTimerInterval(timer, interval)    { timer.it_interval.tv_sec = 0; timer.it_interval.tv_usec = interval * 1000; }
#define SetTimerTime(timer, timeval)         { timer.it_value.tv_sec = 0; timer.it_value.tv_usec = timeval * 1000; }
#define StopTimer(timer)                     { timer.it_interval.tv_sec = 0; timer.it_interval.tv_usec = 0; timer.it_value.tv_sec = 0; timer.it_value.tv_usec = 0; }
#define GetTimer(timer)                      getitimer(ITIMER_REAL, &timer)
#define GetTimerInterval(timer)              (timer.it_interval.tv_usec / 1000)
#define GetTimerTime(timer)                  (timer.it_value.tv_usec / 1000)

// Pipe
#define TPipe(pipename)                       int pipename[2]
#define CreatePipe(pipename)                  (pipe(pipename) < 0 ? false : true)
#define ReadFromPipe(pipename, buffer, count) read(pipename[0], buffer, count)
#define WriteToPipe(pipename, buffer, count)  write(pipename[1], buffer, count)
#define ClosePipeRead(pipename)               close(pipename[0])
#define ClosePipeWrite(pipename)              close(pipename[1])
#define ClosePipe(pipename)                   { close(pipename[0]); close(pipename[1]); }

char GetByte(int value, int nth)
{
	return (char)((value & (0xFF << (nth * 8))) >> (nth * 8));
}

int MergeBytes2(char byte2, char byte1)
{
	return (((int)(byte2) & 0xFF) << 8) | ((int)(byte1) & 0xFF);
}

int MergeBytes4(char byte4, char byte3, char byte2, char byte1)
{
	return (MergeBytes2(byte4, byte3) << 16) | MergeBytes2(byte2, byte1);
}

// System IPC Key
#define TSysKey                        key_t
#define CreateSysKey(pathname, projid) ftok(pathname, projid) /* -1 error */

// Message Queue
#define MAX_MSG_SIZE 1024
struct __TMsg__
{ 
	long mtype; 
	char mtext[MAX_MSG_SIZE];
};
#define CreateMsgQueue(key)  msgget(key, 0600 | IPC_CREAT) /* -1 error */
#define DeleteMsgQueue(mqid) msgctl(mqid, IPC_RMID, NULL) /* -1 error ; egyszer kell törölni (szülőben) */
int SendMsg(int mqid, char *msg, int length, int type) /* -1 error */
{
	/*int j;
	for (j = 0; j < length; ++j)
		printf("%c", msg[j]);
	printf("\n");*/
	struct __TMsg__ auxmsg; 
	auxmsg.mtype = type;
	/*auxmsg.mtext[0] = GetByte(length, 0);
	auxmsg.mtext[1] = GetByte(length, 1);
	auxmsg.mtext[2] = GetByte(length, 2);
	auxmsg.mtext[3] = GetByte(length, 3);*/
	int i; 
	for (i = 0; i < length; ++i) 
		auxmsg.mtext[i/* + 4*/] = msg[i];
	return msgsnd(mqid, &auxmsg, length, 0/*IPC_NOWAIT*/);
}
int ReceiveMsg(int mqid, char **msg, int *length, int type) /* -1 error */
{
	struct __TMsg__ auxmsg;
	int status = msgrcv(mqid, &auxmsg, MAX_MSG_SIZE, type, 0/*IPC_NOWAIT*/);
	if (status < 0)
		return status;
	int l = status;//MergeBytes4(auxmsg.mtext[3], auxmsg.mtext[2], auxmsg.mtext[1], auxmsg.mtext[0]);
	/*int j;
	for (j = 0; j < l; ++j)
		printf("%c", auxmsg.mtext[j]);
	printf("\n");*/
	*length = l;
	*msg = NewArray(char, l);
	int i; 
	for (i = 0; i < l; ++i) 
		auxmsg.mtext[i/* + 4*/] = (*msg)[i];
	return status;
}
int ReceiveFirstMsg(int mqid, char **msg, int *length) /* -1 error */
{
	return ReceiveMsg(mqid, msg, length, 0);
}

// Shared Memory
#define CreateShMem(key, size)         shmget(key, size, IPC_CREAT | S_IRUSR | S_IWUSR) /* -1 error */
#define DeleteShMem(shmemid)           shmctl(shmemid, IPC_RMID, NULL) /* -1 error ; egyszer kell törölni (szülőben) */
#define GetShMemPtr(shmemid)           shmat(shmemid, NULL, 0) /* char* ; (void*)(-1) error */
#define DeleteShMemPtr(ptr)            shmdt(ptr) /* -1 error */

// Semaphore
#define CreateSemaphore(key)            semget(key, 1, IPC_CREAT | S_IRUSR | S_IWUSR) /* -1 error */
#define SetSemaphoreValue(semid, value) semctl(semid, 0, SETVAL, value) /* -1 error ; 2. param: sem sorszám? */
#define SEMAPHORE_OP_INC                1
#define SEMAPHORE_OP_DEC                -1
int SemaphoreOp(int semid, int psemop) { struct sembuf muvelet; muvelet.sem_num = 0; muvelet.sem_op = psemop; muvelet.sem_flg = 0; return semop(semid, &muvelet, 1); } /* -1 error */
int SemaphoreWait(int semid)           { struct sembuf muvelet; muvelet.sem_num = 0; muvelet.sem_op = SEMAPHORE_OP_DEC; muvelet.sem_flg = 0; return semop(semid, &muvelet, 1); } /* -1 error */
int SemaphorePost(int semid)           { struct sembuf muvelet; muvelet.sem_num = 0; muvelet.sem_op = SEMAPHORE_OP_INC; muvelet.sem_flg = 0; return semop(semid, &muvelet, 1); } /* -1 error */
#define DeleteSemaphore(semid)          semctl(semid, 0, IPC_RMID) /* -1 error ; egyszer kell törölni (szülőben) */

//--------------------------------------------------------------------------------------------------

#define ADDR_MAX_SIZE 1024

TPipe(channel);

void ParentTask(int child_pid, char *pathname);
void ChildTask(char *addr, int addrl);

int SignalHandler(int integer)
{ }

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("ERROR: Not enough arguments!\n");
		exit(EXIT_FAILURE);
	}
	int addrl = strlen(argv[1]) + 1;
	char *addr = NewArray(char, addrl);
	stpcpy(addr, argv[1]);
	addr[addrl] = '\0';
	
	TSigHndl sighndl;
	InitSigHndl(sighndl, SignalHandler);
	SetSigHndl(sighndl, SIGTERM);
	SetSigHndl(sighndl, SIGUSR1);
	
	if (!CreatePipe(channel))
	{
		printf("ERROR: Could not create pipe!\n");
		exit(EXIT_FAILURE);
	}
	
	TProcID child_pid = CreateProcess();
	if (!IsSucceeded(child_pid))
	{
		printf("ERROR: Could not create child process!\n");
		exit(EXIT_FAILURE);
	}
	if (IsParent(child_pid))
		ParentTask(child_pid, argv[0]);
	else
		ChildTask(addr, addrl);
	
	Delete(addr);
	
	return 0;
}

void ParentTask(int child_pid, char *pathname)
{
	printf("Parent: Strat...\n");
	
	printf("Parent: Waiting for signal...\n");
	Suspend(SIGTERM);
	printf("Parent: Signal received!\n");
	char *addr = NewArray(char, ADDR_MAX_SIZE);
	int addrl = 0;
	if (0 > (addrl = ReadFromPipe(channel, addr, ADDR_MAX_SIZE)))
	{
		printf("ERROR: Could not read from pipe!\n");
		SendSignal(child_pid, SIGKILL);
		WaitForChildren();
		exit(EXIT_FAILURE);
	}
	printf("Parent: addr is %s\n", addr);
	
	printf("Parent: creating child2...\n");
	TSysKey key = CreateSysKey(pathname, 1);
	int mq = CreateMsgQueue(key);
	TProcID child2_pid = CreateProcess();
	if (!IsSucceeded(child2_pid))
	{
		printf("ERROR: Could not create child process!\n");
		SendSignal(child_pid, SIGKILL);
		WaitForChildren();
		exit(EXIT_FAILURE);
	}
	if (IsParent(child2_pid))
	{
		printf("Parent: sending msg to child2...\n");
		char *msg = NewArray(char, 4 + addrl);
		msg[0] = GetByte(child_pid, 0);
		msg[1] = GetByte(child_pid, 1);
		msg[2] = GetByte(child_pid, 2);
		msg[3] = GetByte(child_pid, 3);
		int i;
		for (i = 0; i < addrl; ++i)
			msg[4 + i] = addr[i];
		if (0 > SendMsg(mq, msg, 4 + addrl, 42))
		{
			printf("ERROR!!!!\n");
		}
for (i = 0; i < 4 + addrl; ++i)
	printf("%c", msg[i]);
printf("\n");
		Delete(msg);
		printf("Parent: msg sent!\n");
		
		printf("Parent: Waiting for signal...\n");
		Suspend(SIGUSR1);
		printf("Parent: Signal received!\n");
		
		printf("Parent: receiving msg from child2...\n");
		msg = NULL;
		int l;
		ReceiveMsg(mq, &msg, &l, 42);
		printf("Parent: child2: %s\n", msg);
		Delete(msg);
		
		waitpid(child2_pid, NULL, 0);
		DeleteMsgQueue(mq);
		printf("Parent: ok!\n");
		
		Delete(addr);
		ClosePipe(channel);
		WaitForChildren();
		printf("Parent: Done!\n");
	}
	else
	{
		printf("Child2: Strat...\n");
		printf("Child2: Sleep 1...\n");
		Sleep(1);
		
		printf("Child2: Receiving msg from parent...\n");
		char *msg = NULL;
		int l;
		ReceiveMsg(mq, &msg, &l, 42);
		int caddrl = l - 4;
		char *caddr = NewArray(char, caddrl);
		int i;
		for (i = 0; i < addrl; ++i)
			caddr[i] = msg[4 + i];		
		int cpid = MergeBytes4(msg[3], msg[2], msg[1], msg[0]);
		Delete(msg);
		printf("Child2: pid is %d; addr is %s\n", cpid, caddr);
		Delete(caddr);
		
		printf("Child2: sending msg to parent...\n");
		char answ[] = "Roger that!";
		SendMsg(mq, answ, strlen(answ) + 1, 42);
		
		printf("Child2: Send signal...\n");
		SendSignal(GetParentProcID(), SIGUSR1);
		printf("Child2: Signal sent!\n");
		
		printf("Child2: Done!\n");
	}
}

void ChildTask(char *addr, int addrl)
{
	printf("Child: Strat...\n");
	printf("Child: Sleep 1...\n");
	Sleep(1);
	
	printf("Child: Write addr to pipe...\n");
	WriteToPipe(channel, addr, addrl);
	printf("Child: Send signal...\n");
	SendSignal(GetParentProcID(), SIGTERM);
	printf("Child: Signal sent!\n");
	
	ClosePipe(channel);
	printf("Child: Done!\n");
}
