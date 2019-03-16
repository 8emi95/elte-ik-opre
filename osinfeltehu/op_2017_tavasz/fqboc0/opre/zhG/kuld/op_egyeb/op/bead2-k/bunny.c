#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <mqueue.h>

void handler (int signumber)
{
	printf("\nExpedition arrived - signal with number %i has arrived.\r\n", signumber);
}

void handler2 (int signumber, siginfo_t* info, void* nonused)
{
	printf("\nSignal with number %i has arrived.\n", signumber);
	char* buffer;
	switch (info->si_code)
	{
		case SI_USER:
			printf("Expedition (PID number %i) sent the signal manually.\n", info->si_pid);
			break;
		case SI_QUEUE:
			printf("Signal was sent with a value from expedition:\n");
			buffer = (char*)(info->si_value.sival_ptr);
			printf("ID: '%s'\n", buffer);
			break;
		default:
			printf("It was sent by something else.\n");
			break;
	}
	
	printf("\nExpedition is ready to work!\n");
}

int main (int argc, char* argv[])
{
	if (argc != 3)
	{
		perror("Give the following as command line arguments:\n Number of expeditions, time of observation!");
		exit(1);
	}
	
	
	int childs_number = atoi(argv[1]);
	int exp_time = atoi(argv[2]);
	
	pid_t childs[childs_number];
	int status;
	
	signal(SIGUSR1, handler); //first signal
	
	int pipefd[2]; //pipe for ID
	char my_id[10];
	
	struct sigaction sigact; //second signal
	sigact.sa_sigaction = handler2;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &sigact, NULL);
	
	struct mq_attr my_attr;
	char* mqname="/mydata"; //message queue
	mqd_t mq_des;
	
	my_attr.mq_maxmsg = 4;
	my_attr.mq_msgsize = 100;
	
	mq_unlink(mqname);
	mq_des = mq_open(mqname, O_CREAT|O_RDWR, 0600, &my_attr);
	if (mq_des < 0)
	{
		perror("Message queue opening: error occured.\n");
		exit(1);
	}
	
	if (pipe(pipefd) == -1)
	{
		perror("Pipe opening was not successful.\n");
		exit(1);
	}
	
	
	
	pid_t ch = fork();
	if (ch < 0) //fork error
	{
		perror("Fork calling was not successful.\n");
		exit(1);
	}
	else if (ch == 0) //child process
	{
		printf("Expedition begins..\r\n");
		
		sleep(3);
		kill(getppid(), SIGUSR1); //first signal
		//printf("Signal sent by %i expedition\n", i+1);
		
		close(pipefd[1]);
		read(pipefd[0], my_id, sizeof(my_id)); //pipe for ID
		printf("\nID arrived: '%s'\r\n", my_id);
		close(pipefd[0]);
		
		sleep(3);
		union sigval my_sigval;
		my_sigval.sival_ptr = (void*)(my_id);
		sigqueue(getppid(), SIGUSR2, my_sigval); //second signal
		
		//printf("Signal sent by %i expedition\n", i+1);
		
		srand(time(NULL));
		int r = (rand()%4) + 2; // 2, 3, 4 or 5 sec (from 0, 1, 2 or 3)
		printf("\nSending data to parent in %i sec..\n", r);
		sleep(r);
		
		char bunny1[100] = "Data about children bunnies.."; //message queue
		char bunny2[100] = "Data about adult bunnies..";
		char bunny3[100] = "Data about old bunnies..";
		
		int x1 = mq_send(mq_des, bunny1, 100, 1);
		int x2 = mq_send(mq_des, bunny2, 100, 2);
		int x3 = mq_send(mq_des, bunny3, 100, 3);
		int x4 = mq_send(mq_des, my_id, 10, 4);
		if ((x1 < 0) || (x2 < 0) || (x3 < 0) || (x4 < 0))
		{
			perror("Sending message: error occured\n.");
			exit(1);
		}
		
		printf("Message sent to parent.\n");
		sleep(1);
		mq_close(mq_des);
		mq_unlink(mqname);
		
		printf("\nExpedition ended.\r\n");
	}
	else //parent process
	{
		printf("Observation begins..\r\n");
		
		printf("Waiting for signal from expedition..\n");
		pause(); //first signal
		
		close(pipefd[0]);
		printf("\nParent sending ID to expeditions..\n");
		write(pipefd[1], "lion", 5); //pipe for ID
		close(pipefd[1]);
		fflush(NULL);
		
		printf("\nWaiting for singal from expedition..\n");
		pause(); //second signal
		
		char buff1[100], buff2[100], buff3[100], buff4[10]; //message queue
		mq_receive(mq_des, buff1, 100, NULL);
		printf("\nMessage arrived from expedition: '%s'\n", buff1);
		mq_receive(mq_des, buff2, 100, NULL);
		printf("Message arrived from expedition: '%s'\n", buff2);
		mq_receive(mq_des, buff3, 100, NULL);
		printf("Message arrived from expedition: '%s'\n", buff3);
		mq_receive(mq_des, buff4, 10, NULL);
		printf("And ID arrived from expedition: '%s'\n", buff4);
		
		waitpid(ch, &status, 0);
		
		printf("\nObservation ended.\n");
	}
	/*
	int i;
	for (i = 0; i < childs_number; ++i)
	{
		childs[i] = fork();
		if ((childs[i]) < 0) //error
		{
			perror("Fork calling was not successful.\n");
			exit(1);
		}
		else if (childs[i] == 0) //child process
		{
			printf("Expedition %i begins..\n", i+1);
			
			sleep(3);
			kill(getppid(), SIGUSR1);
			printf("Signal sent by %i expedition\n", i+1);
			
			close(pipefd[1]);
			read(pipefd[0],str,sizeof(str));
			printf("ID arrived: '%s'\n", str);
			close(pipefd[0]);
			
			printf("Expedition %i ended.\n", i+1);
		}
		else //parent process
		{
			printf("Waiting for singal from expedition..\n");
			pause();
			int status;
			waitpid(childs[i], &status, 0);
			
			//sleep(2);
			if (pipe(pipefd) == -1)
			{
				perror("Pipe opening was not successful.\n");
				exit(1);
			}
			
			close(pipefd[0]);
			write(pipefd[1], "lion", 5);
			close(pipefd[1]);
			fflush(NULL);
			
			
			
		}
		
	}
	
	pid_t fufufu;
	int i;
	for (i = 0; i < childs_number; ++i)
	{
		fufufu = fork();
		if (fufufu < 0) //error
		{
			perror("Fork calling was not successful.\n");
			exit(1);
		}
		else if (fufufu == 0) //child process
		{
			printf("Expedition %i begins..\n", i);
			
			//sleep(3);
			//kill(getppid(), SIGUSR1);
			//printf("Signal sent by %i expedition\n", i);
			
			printf("Expedition %i ended.\n", i);
		}
		else
		{
			waitpid(fufufu, &status, 0);
		}
	}*/
	
	
	return 0;
}