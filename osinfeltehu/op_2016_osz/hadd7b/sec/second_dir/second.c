// vim:fdm=marker
// Név: Németh Tamás
// Neptun-kód: HADD7B

/* macros{{{*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <time.h>
#include <semaphore.h>
#include <math.h>
#include "e_stack.h"
#include "helpers.h"

#define EVENT_DIR "events/" 
#define DELIMITER ','

#define SEPARATOR "_____________________________________________"
#define TITLE  "King of Strands rendezvényszervező társaság"
#define WELCOME   "\n" TITLE "\n" SEPARATOR "\n\n"
#define SERVE_POSTFIX " rendezvény lebonyolítása megkezdve...\n" 

#define RATE 10
#define SUCC_MSG "A siker mértéke:\n"
#define FORK_FAIL "A forkolás sikertelen volt\n"

#define NINCFD 2
#define NOT_INC "Akik nem érkeztek meg:\n"
#define ALL_ARR "Mindenki megérkezett!\n"

#define LEN(STR) sizeof(STR)
/* }}}*/

/* functions{{{*/
// Innentől a push/pop/init műveletek egy stringeket tároló stack adatszerkezetre vonatkoznak
str_stack* init_stack(char* dir, size_t size)
{
	str_stack* st = init(size);

	DIR* dp = opendir(dir);

	struct dirent *de;

	while((de = readdir(dp)) != NULL)
	{
		if(de->d_name[0] != '.')
		{
			push(st, de->d_name);
		}
	}

	return st;
}

int get_guests(str_stack* st, char* event)
{
	char* cd = concat(EVENT_DIR, event);

	int fd = open(cd, O_RDWR|O_CREAT, S_IRWXU);

	struct stat stptr[1];
	fstat(fd, stptr);

	char buf[stptr->st_size];
	read(fd, buf, stptr->st_size);

	char** gsts = split_str(buf, stptr->st_size, '\n');
	for(; *gsts[0] != '\0'; ++gsts)
	{
		push(st, split_str(*gsts, strlen(*gsts), ',')[0]);
	}

	return 1;
}

void travel()
{
	sleep(0.5);
	return;
}

int organize(str_stack* st)
{
	int succ = 0;
	int gc = top(st);
	str_stack* nst = init(1024);
	struct timeval t1;

	char* crnt;
	while(top(st) > 0)
	{
		crnt = pop(st);
		gettimeofday(&t1, NULL);
		srand(t1.tv_usec * t1.tv_sec);	
		//sleep(1);
		//srand(time(NULL));
		if(rand() % 100 <= RATE)
		{
			push(nst, crnt);
		}
		else
		{
			++succ;
		}
	}
	*st = *nst;

	if(succ > 0)
	{
		return (int)(succ*100/gc);
	}
	else
	{
		return 0;
	}
}

int serve(/*pid_t pid, */str_stack* st)
{
	sem_t mutex;
	sem_init(&mutex, 1, 1);

	int fd[2];
	int fd2[2];
	int pid;

	pipe(fd);
	pipe(fd2);

	if((pid = fork()) < 0)
	{
		write(2, FORK_FAIL, LEN(FORK_FAIL));
	}

	char buf[128] = {'\0'};

/* child {{{*/
	if(pid == 0)
	{
		int byte;
		int succ;

		// Rendezvény bekérése
		close(fd[1]);
		read(fd[0], &byte, sizeof(byte));
		read(fd[0], buf, byte);
		char* event = buf;
		write(1, concat(event, SERVE_POSTFIX), strlen(event) + LEN(SERVE_POSTFIX));
		byte = 0;
		memset(buf, 0, 0);

		// Utazás és jelzés a szülőnek
		travel(event);
		sem_post(&mutex);

		// Résztvevőkre várakozás és kiírás
		str_stack* gs = init(1024);
		sem_wait(&mutex);

		memset(buf, 0, 0);

		while(read(fd[0], &byte, sizeof(byte)))
		{
			read(fd[0], &buf, byte);
			memset(buf, 0, 0);
			push(gs, buf);
		}

		// Rendezvény szervezése
		//float succ = organize(gs);
		succ = organize(gs);
		sem_post(&mutex);

		// Siker értékének küldése
		close(fd[0]);
		close(fd2[0]);
		write(fd2[1], &succ, sizeof(succ));
		sem_wait(&mutex);

		// Nem-megérkezettek küldése
		if(succ < 100)
		{
			char* crnt;
			while(top(gs) > 0)
			{
				crnt = pop(gs);
				int len_crnt = strlen(crnt);
				write(fd2[1], &len_crnt, sizeof(len_crnt));
				write(fd2[1], crnt, len_crnt);
			}
		}

		// Befejezés
		close(fd2[1]);
		exit(0);
	}
/* }}}*/

/* parent{{{*/
	else
	{
		int byte;
		int succ;

		// Rendezvény küldése
		char* event = pop(st);
		close(fd[0]);
		int len_event = strlen(event); 
		write(fd[1], &len_event, sizeof(len_event));
		write(fd[1], event, len_event);

		// Gyerek válaszára várás(utazás)
		sem_wait(&mutex);

		// Vendégek küldése
		str_stack* mgs = init(1024);
		get_guests(mgs, event);
		sem_post(&mutex);

		char* crnt;
		while((crnt = pop(mgs)) > 0)
		{
			int len_crnt = strlen(crnt);
			write(fd[1], &len_crnt, sizeof(len_crnt));
			write(fd[1], crnt, len_crnt);
		}

		// Rendezvény szervezése
		sem_wait(&mutex);

		// Siker értékének fogadása
		close(fd[1]);
		close(fd2[1]);
		read(fd2[0], &succ, sizeof(succ));
		char* ssucc = itoa(succ);
		write(1, concat(concat(SUCC_MSG, ssucc), "\%\n"), LEN(SUCC_MSG) + strlen(ssucc) + 1);
		sem_post(&mutex);

		// Nem-megérkezett vendégek fogadása
		if(succ < 100)
		{
			write(NINCFD, NOT_INC, LEN(NOT_INC));
			while(read(fd2[0], &byte, sizeof(byte)))
			{
				read(fd2[0], &buf, byte);
				write(NINCFD, concat(buf, "\n"), strlen(buf) + 1);
				memset(buf, 0, 0);
			}				
		}
		else
		{
			write(1, ALL_ARR, LEN(ALL_ARR));
		}

		// Befejezés
		close(fd2[0]);
		wait(NULL);
		return 1;
	}
}
/* }}}*/
/* }}}*/

/* main{{{*/
int main(int argc, char** argv)
{
	int fd[2];
	int fd2[2];

	pipe(fd);
	pipe(fd2);

	str_stack* st = init_stack(EVENT_DIR, 128);

	write(1, WELCOME, LEN(WELCOME));

	while(top(st) > 0)
	{
		serve(st);
		write(1, concat(SEPARATOR, "\n"), LEN(SEPARATOR) + 1);
	}

	return 0;
}
/* }}}*/

