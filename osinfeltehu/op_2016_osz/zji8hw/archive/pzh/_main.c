#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <mqueue.h>

#define MSGSIZE 50
#define MAXMSGS 5

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	char *mqname = "/uzenetsor";
	mqd_t mqdes;
	
	struct mq_attr attr;
	attr.mq_maxmsg = MAXMSGS;
	attr.mq_msgsize = MSGSIZE;

	pid_t kevin = fork();
	if (kevin < 0)
	{
		perror("forking");
		exit(1);
	}
	
	if (kevin == 0)
	{
		//printf("Kevin vagyok\n");
		
		int toys = rand() % 31 + 20;

		//mq_unlink(mqname);
		mqdes = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
		if (mqdes < 0) { 
			printf("mq_open error\n"); 
			exit(1);
		} 
		
		char str[15];
		sprintf(str, "%d", toys);
		
		mq_send(mqdes, str, sizeof(str), 5);
		printf("kuldom --> %s\n", str);
		wait( NULL ); 
	 
		mq_close(mqdes);
	}
	else
	{
		pid_t betoro = fork();
		if (betoro < 0)
		{
			perror("forking");
			exit(1);
		}
		
		if (betoro == 0)
		{
			//printf("Betoro vagyok\n");
			char buffer[100];
			//mq_unlink(mqname);
			mqdes = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
			if (mqdes < 0) { 
				printf("mq_open error\n"); 
				exit(1);
			} 
			sleep(1);
			printf("recv elott\n");
			mq_receive(mqdes, buffer, MSGSIZE, 0);
			//printf("recv utan\n");
			printf("ez jott: %s\n", buffer);
			mq_close(mqdes);
		}
		else
		{
			//printf("Szulo vagyok\n");
		}
	}
}