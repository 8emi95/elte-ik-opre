#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>

struct uzenet
{
	long mtype;
	char mtext[1024];
};

void sighandler( int sig )
{
	if ( sig == SIGRTMIN )
		puts( "valaki érti..." );
	if ( sig == SIGRTMIN+1 )
		puts( "valaki nem érti..." );
}

int main( int argc, char **argv )
{
	srand( time(0) );
	key_t kulcs = ftok( argv[0], 1 );
	int uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );
	int pipefd[2];
	pipe( pipefd );
	pid_t child1;
	pid_t child2;

	child1 = fork();
	if ( child1 < 0 ) {
		puts("child1 fork hiba");
		return 0;
	}
	if ( child1 > 0 ) {
		//szulo
		child2 = fork();
		if ( child2 == 0 ) {
			sleep( 1 );
			struct uzenet fog;
			msgrcv( uzenetsor, &fog, 1024, 1, 0 );
			printf( "ch2 kapta: %s\n", fog.mtext );
			int randnum = 1 + rand() % 4;
			write( pipefd[1], (char*)&randnum, sizeof(int) );
			signal( SIGTERM, sighandler );
			pause();
			kill( getppid(), SIGRTMIN+(rand()%2) );
			puts( "ch2 vege..." );
		} else {
			signal( SIGRTMIN, sighandler );
			signal( SIGRTMIN+1, sighandler );
			struct uzenet msg = { 1, "Kerdes a hallgatoknak1" };
			puts( "szulo kuld" );
			msgsnd( uzenetsor, &msg, strlen(msg.mtext)+1, 0 );
			msgsnd( uzenetsor, &msg, strlen(msg.mtext)+1, 0 );
			int a, b;
			read(pipefd[0], &a, sizeof(int) );
			read(pipefd[0], &b, sizeof(int) );
			printf( "valasz1: %d\nvalasz2: %d\n", a, b );
			sleep( 1 );
			kill( child1, SIGTERM );
			kill( child2, SIGTERM );
			pause();
		}
	} else {
		sleep( 2 );
		struct uzenet fog;
		msgrcv( uzenetsor, &fog, 1024, 1, 0 );
		printf( "ch1 kapta: %s\n", fog.mtext );
		int randnum = 1 + rand() % 4;
		randnum = 1 + rand() % 4;
		write( pipefd[1], (char*)&randnum, sizeof(int) );
		signal( SIGTERM, sighandler );
		pause();
		kill( getppid(), SIGRTMIN+(rand()%2) );
		puts( "ch1 vege..." );
	}
}
