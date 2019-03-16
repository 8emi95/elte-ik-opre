#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

void handler(int signumber){
	//printf("Jott egy uzenet\n");
}

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

int BmfKuld( int uzenetsor, int metroToday ){ 
	struct uzenet uz;
	uz.mtype = 5;
	char msg[10];
	sprintf(msg, "%i", metroToday);
	strcpy(uz.mtext, msg); 
	int status; 
	 
	status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 );
	if ( status < 0 ) 
		perror("msgsnd"); 
	return 0; 
} 

int MetroKuld( int uzenetsor, char msg[] ){ 
	struct uzenet uz;
	uz.mtype = 5;
	strcpy(uz.mtext, msg); 
	int status;
	 
	status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 );
	if ( status < 0 ) 
		perror("msgsnd"); 
	return 0; 
} 

int MetroFogad( int uzenetsor ) 
{ 
    struct uzenet uz; 
    int status;
    status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
    
	int number;
    if ( status < 0 ) 
        perror("msgsnd"); 
    else{
		number = atoi(uz.mtext);
		printf("Metro: A sorszamom %i\n", number);
	}
	
	int max;
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
    if ( status < 0 ) 
        perror("msgsnd"); 
    else{
		max = atoi(uz.mtext);
		printf("Metro: Osszesen ennyi szerelveny van:  %i\n", max); 
	}
	
	int metroLeft = max-number;
	printf("Metro: Utanam meg ennyi szerelveny van: %i\n",metroLeft);
	
	int succes = rand()%10;
	if(succes==0){
		printf("Metro: Jajj egy bananhej!!!\n");
		MetroKuld(uzenetsor,"Metro: nem teljesitettem az utat.");
		kill(getppid(),SIGTERM);
	} else {
		printf("Metro: Nem volt bananhej.\n");
		MetroKuld(uzenetsor,"Metro: teljesitettem az utat."); //
		kill(getppid(),SIGTERM);
	}
	return 0;
} 

int BmfFogad( int uzenetsor ) 
{ 
    struct uzenet uz; 
    int status; 
    status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
    if ( status < 0 ) 
        perror("msgsnd"); 
    else {
		printf("BMF: A metrotol ezt kaptam: %s\n",uz.mtext);
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
	key_t kulcs; 

	struct sigaction sa;
	sa.sa_handler=handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	sigaction(SIGTERM,&sa,NULL);

	kulcs = ftok(argv[0],1);
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	
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
		int metroToday = (rand()%max)+1;
		BmfKuld(uzenetsor,metroToday);
		BmfKuld(uzenetsor,max);
		sigset_t sigset;					
		sigfillset(&sigset);				
		sigdelset(&sigset,SIGTERM);			
		sigsuspend(&sigset);
		BmfFogad(uzenetsor);
		wait(NULL);
		status = msgctl( uzenetsor, IPC_RMID, NULL ); 
	} 
	else {	// Gyerek
		MetroFogad(uzenetsor); 
	}
	
	return 0;
}