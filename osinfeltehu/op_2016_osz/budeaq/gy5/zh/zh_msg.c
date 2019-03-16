#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

int kuld( int uzenetsor, pid_t pid, char* msg ){ 
	struct uzenet uz;
	uz.mtype = pid; 
	strcpy(uz.mtext, msg); 
	int status; 
	 
	status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 );
	if ( status < 0 ) 
		perror("msgsnd"); 
	return 0; 
} 

int fogad( int uzenetsor ) 
{ 
    struct uzenet uz; 
    int status; 
    status = msgrcv(uzenetsor, &uz, 1024, getpid(), 0 ); 
     
    if ( status < 0 ) 
        perror("msgsnd"); 
    else{
        printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
	}
	return 0;
} 

int main(int argc,char ** argv){
	if(argc!=2){
		printf("A program egy parametert var: jaratok szama\n");
		exit(1);
	}
	
	int max=atoi(argv[1]);
	printf("Szerelvenyek szama: %i\n", max);
	
	srand ( time(NULL) );
	pid_t pid;
	int uzenetsor, status;
	
	uzenetsor = msgget( IPC_PRIVATE, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) {
        perror("msgget"); 
        return 1; 
    }
	
	pid = fork();
	if (pid == -1) {
		perror("Fork hiba");
		exit(1);
	}
	if (pid > 0){	// Szulo
		// Hanyadik szerelveny szamitasa
		int metroToday = (rand()%max)+1;
		//int metroLeft = max-metroToday;
		//printf("BMF: A mai nap te a(z) %i. metro vagy.\n", metroToday);
		char msg[100];
		//sprintf(msg, "Kedves metro! A mai nap te a(z) %i. szerelveny vagy. Meg %i lesz utanad.", metroToday, metroLeft);
		sprintf(msg, "%i", metroToday);
		kuld( uzenetsor, pid, msg );
		
		// Szerelveny uzenete
		/*char* msg;
		read(pipecw[0],&msg,sizeof(msg));
		printf("BMF: %s",msg);*/
	} 
	else {	// Gyerek
		// Hanyadik szerelveny ma
		int metroToday = 0;
		char msg[100] = fogad( uzenetsor );
		printf("%s", msg);
		/*read(pipepw[0],&metroToday,sizeof(metroToday));
		int metroLeft = max-metroToday;
		printf("Metro: A mai nap én vagyok a(z) %i. metro. Utanam meg %i lesz.\n", metroToday, metroLeft);
		int succes = rand()%10;
		char* msg;
		if(succes==0){
			printf("Metro: Jajj ne, egy bananhej!!!\n");
			printf("Metro: Nem teljesitettem sikeresen az utat.\n");
			msg = "A metro nem teljesitette az utat, mert elcsuszott egy bananhejon.\n";
			
		} else {
			printf("Metro: Sikeresen teljesitettem az utat.\n");
			msg = "A metro sikeresen teljesitette az utat, nem csuszott el bananhejon.\n";
		}
		write(pipecw[1],&msg,sizeof(msg));*/
	}
	
	return 0;
}