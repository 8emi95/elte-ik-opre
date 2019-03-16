#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MESSAGEQ_NAME "/mmq"
#define MAX_MSG_LEN 2048

typedef struct msgbuf{
	long type;
	char text[MAX_MSG_LEN];
}msgbuf;

int main(int arg, char *args[]){
	static mqd_t my_mq;
	key_t key = 13579;
	int msqid;
	msqid = msgget(key, 0776); 
	
	if(msqid < 1){
		perror("msgget\n");
		exit(-1);
	}

	msgbuf buf;
	buf.type = 1;
	
	int status;
	printf("Client opening msq\n");
	status = mq_open(MESSAGEQ_NAME, O_RDWR);

	if(status < 0){
		perror("mq_open");
		exit(-1);
	}

	printf("Client receving message\n");
	//status = mq_receive(my_mq, in_msg, MAX_MSG_LEN, NULL);
	status = msgrcv(msqid, &buf, sizeof(buf), 1, 0);
	if(strlen(buf.text) < 3)
		printf("Hibás üzenet.\n");
	printf("Client recieved message.\n");
	if(status < 0){
		perror("mq_receive");
		exit(-1);
	}else{
		printf("valami...\n");
	}
	printf("Server üzenete: %s\n", buf.text);

	strcpy(buf.text, "Megkaptam az üzenetet. Client ki.\n");
	status = msgsnd(msqid, &buf, 100, 1);
	if(status < 0){
		perror("mq_send");
		exit(-1);
	}

	while(1){
		status = msgrcv(msqid, &buf, sizeof(buf), 1, 0);
		if(strncmp(buf.text, "cvege", 4) == 0)
			break;
		printf("Client: %s\n", buf.text);
	}
	printf("Client finished.\n");

	return 0;
}
