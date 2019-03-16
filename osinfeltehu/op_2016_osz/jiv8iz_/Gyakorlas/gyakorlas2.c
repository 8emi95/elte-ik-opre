#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 

#include <string.h> 
#include <unistd.h> 
#include <wait.h>

struct msg{
	long mtype;
	char mtext[256];
};

int main(int argc, char** argv){

	
	int pipefd[2];
	pid_t gyerek1,gyerek2;
	
	key_t key = ftok(argv[0],1);
	int msgid = msgget(key,IPC_CREAT | 0666);
	
	//srand(time(NULL));
	//srand(time(NULL) ^ (getpid()<<16));

	
	if(pipe(pipefd) == -1){
		printf("Pipe error\n");
		exit(EXIT_FAILURE);
	}
	
	gyerek1 = fork();
	
	if(gyerek1 == 0)
	{// elso gyerek
		struct msg uzenet;
		msgrcv(msgid, &uzenet,256,1,0);
		printf("ELSO: Megkaptam az uzenetet: %s\n",uzenet.mtext);
		//sleep(1);
		//srand(time(NULL) ^ (getpid()<<16));
		srand(time(NULL) +100 * getpid());
		//srand(time(NULL));
		int valasz = rand() % 4+1;
		char val[100];
		strcpy(val,"2");
		//itoa(valasz,val,10);
		write(pipefd[1], &valasz, sizeof(valasz));
		printf("ELSO: Elkuldtem a valaszomat.\n") ;
	
	}else
	{
		gyerek2 = fork();
	
		if(gyerek2 == 0)
		{ //masodik gyerek
			printf("Masodik, megszulettem\n");
			struct msg uzenet;
			msgrcv(msgid, &uzenet,256,1,0);
			//sleep(2);
			printf("megvan");
			printf("MASODIK: Megkaptam az uzenetet: %s\n",uzenet.mtext);
			//srand(time(NULL) ^ (getpid()<<16));
			srand(time(NULL) + 100*getpid());
			int valasz = rand() %4+1;
			write(pipefd[1], &valasz, sizeof(valasz));
			printf("MASODIK: Elkuldtem a valaszomat.\n") ;
		
		}else
		{ // Szulo
		
			struct msg uzenet = {1, "Mennyi 1+1?"};
			msgsnd(msgid,&uzenet,strlen(uzenet.mtext)+1,0);
			msgsnd(msgid,&uzenet,strlen(uzenet.mtext)+1,0);

			printf("Elkuldtem a gyerekeknek a kerdest.\n");
			//wait();
			char val[100];
			int valasz;
			int valasz1;

			read(pipefd[0], &valasz, sizeof(valasz));
			read(pipefd[0], &valasz1, sizeof(valasz1));

			//char val[100] = itoa(valasz);
			wait(0);
			printf("ELSOTOL: Megkaptam a valaszt: %i\n",valasz);
			printf("MASODIKTOL: Megkaptam a valaszt: %i\n",valasz1);

			waitpid(gyerek1, NULL, 0);
			waitpid(gyerek2, NULL, 0);
			
		}
	
	}




}