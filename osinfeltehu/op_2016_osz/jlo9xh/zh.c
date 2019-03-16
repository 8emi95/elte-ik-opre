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

int main()
{
	int uzenetsor, status; 
	key_t kulcs;
	
	kulcs = ftok("asd",1);
	
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
        perror("msgget"); 
        return 1; 
    }
	

	pid_t gyerek1 = fork();
	
	if(gyerek1 > 0)
	{
		pid_t gyerek2 = fork();
		if(gyerek2 == 0)
		{
			struct uzenet uz;
			
			srand(time(NULL));
			int rn=(rand() % 30)+20;

			uz.mtype=1;
			uz.num=rn;
			
			printf("Kevin: koldom a jatekokat (%i)\n",rn);
			status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
			
			sleep(3);
			
			status = msgrcv(uzenetsor, &uz, sizeof(uz), 2, 0 );
			printf("Kevin: %s\n",uz.mtext);
			
			status = msgctl( uzenetsor, IPC_RMID, NULL ); 
			if ( status < 0 ) 
               perror("msgctl"); 
		}
		else
		{
			int st;
			wait(&st);
			wait(&st);
		}
		
	}
	else
	{
		sleep(3);
		
		struct uzenet uz;

		status = msgrcv(uzenetsor, &uz, sizeof(uz), 1, 0 );
		printf("Betoro: enyi jatek volt: %i\n",uz.num);
		printf("Betoro: AUUUUUUUUUUUU!!!!!!!!!!\n");
		
		uz.mtype=2;
		strcpy(uz.mtext,"csak varj, mig a kezunk koze nem kerulsz!");
		printf("Betoro: %s\n",uz.mtext);
		status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 );
		
		
	}
	
	
}