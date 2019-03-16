//Fordítás -lrt-vel, message queue megölés: ipcrm -Q key_value
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MESSAGEQ_NAME "/mmq"
#define MAX_MSG_LEN 2048

typedef struct msgbuf{
	long type;
	char text[MAX_MSG_LEN];
}msgbuf;

int main(int arg, char *args[]){

  static struct mq_attr my_mq_attr;
  static mqd_t my_mq;
  key_t key = 13579;
  int msgq_id;
  msgq_id = msgget(key, IPC_CREAT | 0766);
  
  if(msgq_id < 1){
    perror("msgget");
    exit(-1);
  }

  printf("MessagaQ id: %i\n", msgq_id);

  my_mq_attr.mq_maxmsg = 100;
  my_mq_attr.mq_msgsize = MAX_MSG_LEN*2;

  my_mq = mq_open(MESSAGEQ_NAME, O_CREAT | O_RDWR, 0777, &my_mq_attr);
  
  int status;
  //char *message = "Hello Client";
  msgbuf buf;
  buf.type = 1;

  strcpy(buf.text, "Init...");
  status = mq_send(my_mq, "Init..", MAX_MSG_LEN, 1);
 
  strcpy(buf.text, "Hello Client.");
  printf("Server sending msg.\n");
  status = msgsnd(msgq_id, &buf, 100, 0);

  if(status < 0){
	printf("msgsnd error...\n");
	perror("msgsnd");
	exit(-1);
  }

  printf("Server sent message.\n");

  char quit[] = "vege";

  // printf("Server message check: %s\n", buf.text);
  while(1){
    sleep(1);
    printf("Enter message\n");
    fgets(buf.text, 200, stdin);
    status = msgsnd(msgq_id, &buf, 200, 0);
    if(strncmp(buf.text, "vege", 4) == 0)
	break;
  }

  printf("Server finished.\n");
  return 0;
}

//FONTOS    fordítás végére kell a "-lrt" undefined refferemce to mq_open és undefined refference to mq_read... miatt
