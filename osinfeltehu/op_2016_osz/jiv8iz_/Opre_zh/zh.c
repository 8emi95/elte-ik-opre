#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h> 
#include <unistd.h> 
#include <wait.h>


struct msg{
	long mtype;
	char mtext[100];
	int max;
	int akt;
	
};

int joe;

void handler1(int n) 
{
	printf("SIGNAL: Vegallomason vagyok, beertem!\n");
}

void handler2(int n) 
{
	printf("SIGNAL: Vegallomason vagyok, bevontattak!\n");
}


int main(int argc, char* argv[])
{
		
	int szerelvenyek;
	szerelvenyek =atoi(argv[1]);
	
	printf("A beolvasott szam: %d\n",szerelvenyek);
	
	pid_t gyerek;
	key_t key = ftok(argv[0],1);
	int msgid = msgget(key, IPC_CREAT | 0666);
	
	srand(time(NULL));
	
	signal(1,handler1);
	signal(2,handler2);
		
	gyerek = fork();
	
	if(gyerek == -1)
	{
		printf("Hiba a gyerek letrehozasanal\n");
		exit(EXIT_FAILURE);
	}
	
	if(gyerek == 0) // Elso gyerek
	{
		//printf("En vagyok a gyerek, pidem: %d\n",getpid());
		struct msg uzenet;
		
		msgrcv(msgid, &uzenet,sizeof(uzenet),1,0);
		int sajatmax = uzenet.max;
		int sajatakt = uzenet.akt;
		int rossze;
		char valasz[100];
		rossze = rand() % 100 +1;
		if(rossze <=10){
			strcpy(valasz,"Elromlottam\n");
			joe = 0;
		}else {
			strcpy(valasz,"Nincsen bajom\n");	
			joe	= 1;		
		}
				
		printf("GYEREK: Megkaptam az uzenetet, osszese szerelveny:%d , en az %d. vagyok\n",sajatmax,sajatakt);
		printf("GYEREK: Szazalekom: %d\n",rossze);
		printf("GYEREK: Hanyadik vagyok: %d\nHanyan vannak mogottem: %d\nJo vagyok-e: %s",sajatakt,sajatmax-sajatakt,valasz);
		printf("GYEREK: Jo vagyok e (1/0): %d\n",joe);
		
		struct msg valaszok = {.mtype = 1, .mtext = "", .max = 0, .akt = 0};
		
     	if(joe == 1){
			
			kill(getppid(),1);
			
			strcpy(valaszok.mtext, "Beertem, nincs problema.\n");
			
			
		}else{
			
			kill(getppid(),2);
			
			strcpy(valaszok.mtext, "Lefagytam a hidegben\n");
			
			
		}
		printf("GYEREK: elkuldtem a szignalt\n");
		
		msgsnd(msgid, &valaszok,sizeof(valaszok),0);
		
		printf("GYEREK: elkuldtem aZ okot\n");
		
		
		
		
		
		
	}else //Szulo
	{
		//printf("En vagyok a szulo, pidem: %d\n",getpid());
		
		
		int aktszer;
		aktszer = rand() % szerelvenyek +1;
		printf("SZULO: Osszes: %d\nAktualis: %d\n",szerelvenyek,aktszer);
		struct msg uzenet = {.mtype = 1, .mtext = "", .max = szerelvenyek, .akt = aktszer};
		
		msgsnd(msgid, &uzenet, sizeof(uzenet),0);
		printf("SZULO: Elkuldtem az uzenetet, max szerelveny: %d es akt szerelveny: %d\n",szerelvenyek,aktszer);
		pause();
		printf("SZULO: Megkaptam a szignalt.\n");
		
		struct msg valaszok;
		msgrcv(msgid,&valaszok,sizeof(valaszok),1,0);
		printf("SZULO: A gyerek valasza, ha elakadt: %s",valaszok.mtext);
		
		waitpid(gyerek,NULL,0);
		msgctl(msgid, IPC_RMID, NULL); // felszabaditom a sort
	}
}