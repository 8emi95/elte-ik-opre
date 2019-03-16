#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <mqueue.h>

#define MAXMSGS 5
#define MSGSIZE 100

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
		
		char str[4];
		sprintf(str, "%d", toys);
		
		printf("Kevin: a betorok ele szorok %s jatekot!\n", str);
		mq_send(mqdes, str, 4, 1);
		//wait(NULL);
		
		sleep(2);
		
		char buffer[100];
		mq_receive(mqdes, buffer, 50, NULL);
		printf("Kevin kapja: %s\n", buffer);
		
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
			mq_receive(mqdes, buffer, 4, NULL);
			printf("Betoro: hanyatt esek a %s jatekon!\n", buffer);
			
			sleep(1);
			
			char buf[50];
			strcpy(buf, "Csak varj, amig a kezunk koze kerulsz");
			//printf("%s", buf);

			mq_send(mqdes, buf, 50, 1);
			//wait(NULL);

			mq_close(mqdes);
		}
		else
		{
			//printf("Szulo vagyok\n");
		}
	}
}