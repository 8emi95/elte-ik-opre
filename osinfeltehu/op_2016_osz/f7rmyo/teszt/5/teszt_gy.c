#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
 
struct uzenet { 
    long mtype;
    int num;
	char mtext [ 1024 ]; 
};
 
void handler() {
    printf("Beertem!\n");
}
void handler2() {
    printf("Bevontattak!\n");
}
 
 int main(int argc, char *argv[])
 {
	int max_szer;
	max_szer=atoi(argv[1]);
	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler2);
	
	int uzenetsor, status; 
    key_t kulcs;
	
	kulcs = ftok(argv[0],1);
	
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
        perror("msgget"); 
        return 1; 
    }
	
	pid_t gyerek = fork();
	
	if(gyerek>0)
	{
	
		struct uzenet uz;
		
		srand(time(NULL));
		int rn=(rand() % max_szer)+1;

		uz.mtype=1;
		uz.num=max_szer;
		status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
		
		uz.mtype=1;
		uz.num=rn;
		status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
		
		pause();
		
		status = msgrcv(uzenetsor, &uz, sizeof(uz), 2, 0 );
		printf("Szulo: %s\n",uz.mtext);
		
		int status;
		wait(&status);
	}
	else
	{
		int max,sorszam;
		sleep(3);

		struct uzenet uz;
		
		status = msgrcv(uzenetsor, &uz, sizeof(uz), 1, 0 );
		printf("Gyerek: %i\n",uz.num);
		max=uz.num;
		
		status = msgrcv(uzenetsor, &uz, sizeof(uz), 1, 0 );
		printf("Gyerek: %i\n",uz.num);
		sorszam=uz.num;
		
		srand(time(NULL));
		int rn=rand() %100;
		if(10 > rn)
		{
			printf("%i. vagyok meg %i van utannam az uzem SIKERTELEN\n",sorszam,max-sorszam);
			uz.mtype=2;
			strcpy(uz.mtext,"Nagy volt a ho!");
			status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
			kill(getppid(), SIGUSR2);
		}
		else
		{
			printf("%i. vagyok meg %i van utannam az uzem SIKERES\n",sorszam,max-sorszam);
			uz.mtype=2;
			strcpy(uz.mtext,"Jo volt az ut!");
			status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
			kill(getppid(), SIGUSR1);
		}
		
		
	}
	return 0;
 }