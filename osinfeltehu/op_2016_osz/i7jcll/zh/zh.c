#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

struct Message {
	long mtype;
	char mtext[1024];
};

int send(int, char[1024], long);
int receive(int, long, char[1024]);
void sig_usr(int);

int main (int argc, char** argv) {
	int msgque; 
	key_t kulcs;
	struct sigaction sig;               
    sigemptyset(&sig.sa_mask);          
    sig.sa_flags = 0;                   
    sig.sa_handler = sig_usr;
	msgque = msgget(kulcs, 0600 | IPC_CREAT); 
	if (msgque < 0) { 
		perror("Message receive failed!"); 
		exit(1); 
	}
	if(argc < 2) {
		perror("Amount of parameters not enough!");
		exit(1);
	}
	else {
		pid_t bmf = getpid();
		pid_t lineOne = fork();
		if (lineOne < 0) {
			perror("lineOne fork failed\n"); 
			exit(1);
		} 
		if (lineOne > 0) {
			pid_t lineTwo = fork();
			if(lineTwo > 0) {//BMF
				//get max number of lines
				int max = atoi(argv[1]);
				//set actual line number
				srand(time(NULL));
				int id = (rand() % max) + 1;
				char text[1024];
				sprintf(text, "%d%s%d", id, ",", max);
				send(msgque, text, 1);
				if(sigaction(SIGINT,&sig,NULL) == 0) {
					pause();
				}
				char arrivalInfo[1024];
				receive(msgque, 1, arrivalInfo);
				printf("Reason sent by line: %s\n", arrivalInfo);
			}
			else { //metro line 2
				//pot szerelveny
			}
		}
		else {//metro line 1
			char text[1024];
			receive(msgque, 1, text);
			char *token;
			int id;
			int max;
			token = strtok(text, ",");
			id = atoi(token);
			token = strtok(NULL, ",");
			max = atoi(token);
			//printf("id: %d, max: %d\n", id, max);
			srand(time(NULL));
			int fail = (rand() % 10) + 1;
			printf("My id is: %d, lines to come after me: %d", id, max - id);
			char reason[1024];
			if(5 == fail) {
				//10% chance it failed
				printf(", failed to complete.\n");
				sprintf(reason, "%s", "It was too cold, failed to complete tour.");
			}
			else {
				printf(", succesfully completed.\n");
				sprintf(reason, "%s", "Everything was OK, managed to complete tour.");
			}
			sleep(2); //wait a bit before arriving to destination
			kill(bmf, SIGINT); //notice BMF
			send(msgque, reason, 1);
		}			
	}
	return 0;
}

int send(int msgq, char text[1024], long mtype) {
	const struct Message msg = {mtype, ""};
	strcpy(msg.mtext, text);
	int status;
	status = msgsnd(msgq, &msg, strlen(msg.mtext)+1, 0); 
	if ( status < 0 ) {
		perror("Send failure!");
		exit(1);
	}
}

int receive(int msgq, long mtype, char text[1024]) { 
    struct Message msg; 
    int status;
    status = msgrcv(msgq, &msg, 1024, mtype, 0); 
    if ( status < 0 ) {
        perror("Send failure!"); 
		exit(1);
	}
	else {
		sprintf(text, "%s",msg.mtext);
	}
} 

void sig_usr(int signo){
	if(signo == SIGINT) {
		printf("Line arrived to station!\n");
	}
	return;
}