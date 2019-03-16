#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define MAXLEN 2048
#define MSGQ_NAME "/mmq"

char *lakcim;
int ppipe[2];

typedef struct msgbuf{
	long type;
	char text[MAXLEN];
}msgbuf;

int main(int arg, char *args[])
{
	static struct mq_attr my_mq_attr;
	static mqd_t my_mq;
	key_t key = 13579;
	int msgqid;
	msgqid = msgget(key, IPC_CREAT | 0766);

	my_mq_attr.mq_maxmsg = 5;
	my_mq_attr.mq_msgsize = MAXLEN*2;

	my_mq = mq_open(MSGQ_NAME, O_CREAT | O_RDWR, 0766, &my_mq_attr);
	msgbuf buf;
	buf.type = 1;

	if(msgqid < 1){
		perror("msgget");
		exit(1);
	}
	if(arg > 1 && arg < 3){
		if(args[1] != NULL)
		{
			lakcim = (char *)malloc(sizeof(char)*strlen(args[1]));
			strcpy(lakcim, args[1]);
			//printf("Check lakcim: %s\n", lakcim);
		}
	}else{
		printf("Hibás argumentum.\n");
		exit(1);
	}

	pid_t utas, taxis;
	pipe(ppipe);
	//taxis = fork();
	if((utas = fork()) < 0){
		perror("fork");
		exit(1);
	}else if(utas == 0){
		//printf("Starting children...\n");
		write(ppipe[1], &lakcim, MAXLEN);
		//sleep(2);
		//printf("Lakcim elküldve. Gyerek vége.\n");
	}else{	
		//printf("Starting parent...\n");
		//while(1){
		  printf("\n");
		
		  wait(&utas);
		  wait(&taxis);
		  kill(taxis, SIGSTOP);
		  //close(ppipe[1]);
		  char *parent_cim = (char *)realloc(NULL, sizeof(char)*MAXLEN);
		  read(ppipe[0], &parent_cim, MAXLEN);
		  kill(utas, SIGSTOP);
		  //close(ppipe[0]);
		  printf("A kapott cím: %s\n", parent_cim);
		  char tel[10];	  
		  sprintf(tel, "%d", utas);
		  strcpy(buf.text, parent_cim);
		  
		  int status = msgsnd(msgqid, &buf, MAXLEN, 0);
		  if(status < 0){
			perror("msgsnd1");
			exit(1);
		  }
		  strcpy(buf.text, tel);
		  status = msgsnd(msgqid, &buf, 300, 0);
		  if(status < 0){
			perror("msgsnd2");
			exit(1);
		  }
		  taxis = fork();
		  wait(&taxis);
		  

		  //kill(taxis, SIGCONT);

		  size_t n;

		  if(taxis < 0){
			printf("Taxis fork hiba.\n");
			exit(1); 
		  }else if(taxis == 0){
		    printf("Taxis elindítva\n");
		    char *taxis_cim = (char *)realloc(NULL, sizeof(char)*MAXLEN);
		    char taxis_tel[10];
		    msgbuf taxis_buf;
		    taxis_buf.type = 1;
		    status = msgrcv(msgqid, &taxis_buf, MAXLEN, 1, 0);
		    strcpy(taxis_cim, taxis_buf.text);
		    status = msgrcv(msgqid, &taxis_buf, 10, 1, 0);
		    strcpy(taxis_tel, taxis_buf.text);
		    printf("Taxis megkapta a címet: %s, tel: %s\n", taxis_cim, tel);
		    sleep(2);
		   
	            printf("Taxis vége");
		    free(taxis_cim);
		  }
		  kill(utas,SIGCONT);
		//}
		printf("Parent vége.\n");
	}
   	free(lakcim);	
	return 0;
}
