#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <wait.h>

// Memory
#define New(type)                    ((type*)(malloc(sizeof(type))))
#define NewArray(type, count)        ((type*)(malloc((count) * sizeof(type))))
#define Delete(ptr)                  free(ptr)
#define ResizeArray(type, ptr, size) ((type*)(realloc(ptr, (size) * sizeof(type))))

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
	struct __TMsg__ auxmsg; 
	auxmsg.mtype = type;
	auxmsg.mtext[0] = GetByte(length, 0);
	auxmsg.mtext[1] = GetByte(length, 1);
	auxmsg.mtext[2] = GetByte(length, 2);
	auxmsg.mtext[3] = GetByte(length, 3);
	int i; 
	for (i = 0; i < length; ++i) 
		auxmsg.mtext[i + 4] = msg[i];
	return msgsnd(mqid, &auxmsg, length, IPC_NOWAIT);
}
int ReceiveMsg(int mqid, char **msg, int *length, int type) /* -1 error */
{
	struct __TMsg__ auxmsg;
	int status = msgrcv(mqid, &auxmsg, MAX_MSG_SIZE, type, IPC_NOWAIT);
	if (status < 0)
		return status;
	int l = MergeBytes4(auxmsg.mtext[3], auxmsg.mtext[2], auxmsg.mtext[1], auxmsg.mtext[0]);
	*length = l;
	*msg = NewArray(char, l);
	int i; 
	for (i = 0; i < l; ++i) 
		auxmsg.mtext[i + 4] = (*msg)[i];
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
int SemaphoreOp(int semid, int semop)  { struct sembuf muvelet; muvelet.sem_num = 0; muvelet.sem_op = semop; muvelet.sem_flg = 0; return semop(semid, &muvelet, 1); } /* -1 error */
int SemaphoreWait(int semid)           { struct sembuf muvelet; muvelet.sem_num = 0; muvelet.sem_op = SEMAPHORE_OP_DEC; muvelet.sem_flg = 0; return semop(semid, &muvelet, 1); } /* -1 error */
int SemaphorePost(int semid)           { struct sembuf muvelet; muvelet.sem_num = 0; muvelet.sem_op = SEMAPHORE_OP_INC; muvelet.sem_flg = 0; return semop(semid, &muvelet, 1); } /* -1 error */
#define DeleteSemaphore(semid)          semctl(semid, 0, IPC_RMID) /* -1 error ; egyszer kell törölni (szülőben) */

