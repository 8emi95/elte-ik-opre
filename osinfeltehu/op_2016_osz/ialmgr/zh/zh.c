#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>

void sig_handler(int signum) {
	printf("[Operator] Train arrived home.\n");
	exit(0);
}

int main(int argc, char* argv[]) {
	int max = atoi(argv[1]);
	char* mqname="/doge";
	char buffer[100];
	struct mq_attr attr;
	mqd_t mq1;
	
	attr.mq_maxmsg = 5;
	attr.mq_msgsize = 50;
	
	mq_unlink(mqname);
	mq1 = mq_open(mqname, O_CREAT | O_RDWR, 0600, &attr);
	
	if(mq1 < 0) {
		perror("MessageQ failed.");
		
		exit(EXIT_FAILURE);
	}
	
	signal(SIGTERM, sig_handler);
	pid_t pid = fork();
	
	if(pid == -1) {
		perror("Fork failed.");
		
		exit(EXIT_FAILURE);
	}
	
	if(pid > 0) {
		char msg[30];;
		int db;
		time_t t;
		srand((unsigned) time(&t));
		int num = rand() % max;
		int remaining = max - num;
		
		sprintf(msg, "%d", num);
		db = mq_send(mq1, msg, 30, 5);
		
		sprintf(msg, "%d", remaining);
		db = mq_send(mq1, msg, 30, 4);
		wait(NULL);
		
		db = mq_receive(mq1, buffer, 54, 0);
		printf("Reason: %s\n", buffer);
		
		mq_close(mq1);
		mq_unlink(mqname);
	} else {
		//child process
		int db;
		time_t tp;
		srand((unsigned) time(&tp));
		int success = rand() % 10;
				
		db = mq_receive(mq1, buffer, 54, 0);
		printf("I am #%s.\n", buffer);
		
		db = mq_receive(mq1, buffer, 50, 0);
		printf("There are %s more trains to come.\n", buffer);
		
		if (success == 6) {
			//winter
			printf("Winter case\n");
			db = mq_send(mq1, "Winter is coming.\n", 30, 6);
		}
		
		if (success == 7) {
			//banana
			printf("Banana case.\n");
			db = mq_send(mq1, "Bloody banana.\n", 30, 6);
		}
		
		if (success == 6 || success == 7) {
			
			mq_close(mq1);
			exit(EXIT_SUCCESS);
		}
		
		kill(getppid(), SIGTERM);
		
		mq_close(mq1);
		exit(EXIT_SUCCESS);
	}
	
	return 0;
}
