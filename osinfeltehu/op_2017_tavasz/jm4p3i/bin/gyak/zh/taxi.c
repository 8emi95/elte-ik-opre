//1 szülõ 2 gyerek
//A) pipe
//utas write to kp cím
//kp printf cím
//B)
//kp mqueue cim + telefon(pid), taxi read, printf, wrtie ack
//C)
//taxi signal to utas, utas signal  1-20 rnd to taxis, taxis printf price 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
//For mqueue
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
//-lrt
int main(int argc, char *argv[])
{
	//Pipe file descriptor
	int pipefd[2];
	//Child processes
    pid_t pid, pid2;
	//Pipe content for read (max 100 chars)
    char address_pipe[100];
	//Message queue descriptors
	mqd_t mq1,mq2;
	//Mq attributes
	struct mq_attr attr;
	//Maximum amount of messages
	attr.mq_maxmsg=5;
	//Message length
	attr.mq_msgsize=50; 
	char* mqname="/messageq";
	char* phone_number;
	
	//Unlink if exists
	mq_unlink(mqname); 
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
    if ( mq1 < 0 ) { 
        perror("mq_open error: %d \n"); 
        return 1; 
    } 
	
	
	if (pipe(pipefd) == -1) 
	{
	    perror("Hiba a pipe nyitaskor!\n");
	    exit(EXIT_FAILURE);
	}
	
	pid = fork();
	switch (pid)
    {
    	case -1:
      		perror("fork hiba\n"); 
			exit(EXIT_FAILURE);
      	case 0: //utas //child
			//A.) Utas
			printf("Utas elindult\n");
			close(pipefd[0]); //close read
			write(pipefd[1], argv[1], strlen(argv[1]) * sizeof(char) );
			printf("Utas elkuldte a cimet a kozpontnak\n");
			close(pipefd[1]); //close write
			printf("Utas vege\n");
			break;
      	default: //kozpont //parent
      		//A.) Kozpont
			printf("Kozpont elindult1\n");
			
			close(pipefd[1]); //close write
			read(pipefd[0], address_pipe, sizeof(address_pipe));
			printf("Kozpont kiolvasta az uzenetet: %s\n", address_pipe);
			close(pipefd[0]);
			wait();
			pid2 = fork();
			switch(pid2)
			{
				case -1:
					perror("2. fork hiba\n");
					exit(EXIT_FAILURE);
				case 0: //taxis //child2
					printf("Taxis elindult\n");
					char buffer_address[100];
					char buffer_phone[100];
					//Receive address from hq
					mq_receive(mq1, buffer_address, 100 * sizeof(char), 0); // get the address
					//mq_close(mq1);
					//mq_receive(mq1, buffer_phone, sizeof(int), 0); // get the phone number
					
					printf("Taxis cimet kapott: %s\n", buffer_address);
					//printf("Taxis telefonszamot kapott: %d\n", (int)buffer_phone);
					mq_close(mq1);
					printf("Taxis vege\n");
					break;
				default: //kozpont //parent
					//Send address to taxi
					mq_send(mq1, address_pipe, strlen(address_pipe) * sizeof(char), 5);
					printf("Kozpont telefonszama: %d\n", pid2);
					//phone_number = (char*)pid2;
					//mq_send(mq1, phone_number, strlen(phone_number) * sizeof(char), 6);
					//close message queue
					mq_close(mq1);
					mq_unlink(mqname);
					wait();
					printf("Kozpont vege\n");
			}
      		
	}
	exit(EXIT_SUCCESS);	
	
}