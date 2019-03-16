#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

int gyerekek = 2;
int kezdo;
	
void handler(int sign){
	printf("");
}

int main(){
	pid_t parentID = getpid();
	pid_t pid;
	pid_t pids[2];
	int p1[2];
	int p2[2];
	char str[512];
		
	signal(SIGPOLL, handler);
	signal(SIGTERM, handler);
	signal(SIGUSR1,handler);

	int uzenetsor, status; 
     key_t kulcs; 
	 kulcs = ftok("/",1); 
     printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 

	
	pipe(p1);
	pipe(p2);
	int i;
	pid=fork();
	pids[0]=pid;
	if(pid > 0) { // Parent
		pid=fork();
		pids[1]=pid;
		if(pid>0){//parent

			printf("A szulo kesz\n");
			for(i=0;i<gyerekek;++i){
				pause();
				printf("Megkaptam egyik jelzest..\n");
			}

			char fiu[512] = "Pisti";
			char kislany[512]="Viki";
			
			close(p1[0]);
			write(p1[1],fiu,sizeof(fiu));

			kill(pids[1],SIGUSR1);

			close(p2[0]);
			write(p2[1],kislany,sizeof(kislany));

			kill(pids[0],SIGUSR1);

			for(i=0;i<gyerekek;++i){
				int st;
				wait(&st);
			}
			printf("Vege a dalnak\n");
			/*for(i=0;i<gyerekek;++i){
				sprintf(str,"%d",kezdo+1);
				
				
				char masik[10];
				sprintf(masik,"%d",pids[(i+1)%gyerekek]);
				sprintf(str,"%d",strlen(masik));
				close(pp[1][0]);
				write(pp[1][1],str,strlen(str)+1);
				
				close(pp[2][0]);
				write(pp[2][1],masik,strlen(masik)+1);
				kill(pids[i],SIGUSR1);
			}
			
			printf("A kezdojatekos a %d. szamu\n",kezdo+1);
			
			for(i=0;i<gyerekek;++i){
				int st;
				wait(&st);
			}
			printf("Vege a sakknak\n");
		*/
		}else{//fiu
			sleep(2);
			printf("  Itt a fiu, jelzek vissza (%d)\n",getpid());
			kill(parentID,SIGUSR1);
			printf("  Most varok a kezdesre..(%d)\n",getpid());
			pause();

			char fiu[512];
			char message[512];
			close(p1[1]);
			read(p1[0],fiu,sizeof(fiu));
			printf("Fogadtam az adatokat!\n");
			printf("Az en nevem: %s\n", fiu);
			printf("Elkuldom a nevem a kislanynak\n");
			msgsnd( uzenetsor, fiu, sizeof(fiu) , 0 );
			printf("Elkuldve\n"); 
			kill(pids[0],SIGUSR1);
			printf("Varok a lany uzenetere\n");
			pause();
			printf("Megjott a lany uzenete\n");
			msgrcv(uzenetsor, message, 512, 0, 0 ); 
			printf("Fiu kapta: %s\n", message);
			printf("En vegeztem is!\n");
		}
		
	}
	else { // kislany
		sleep(1);
		printf("  Itt a lany, jelzek vissza (%d)\n",getpid());
		kill(parentID,SIGUSR1);
		
		printf("  Most varok a kezdesre..(%d)\n",getpid());
		pause();
		
		char lany[512];
		char fiu[512];
		char message[512];
		close(p2[1]);
		read(p2[0],lany,sizeof(lany));
		printf("Fogadtam az adatokat!\n");
		printf("Az en nevem: %s\n", lany);
		printf("Varok a fiu uzenetere\n");
		pause();
		printf("Megjott a fiu uzenete\n");
		msgrcv(uzenetsor, fiu, 512, 0, 0 ); 
		printf("Lany kapta: %s\n", fiu);
		printf("Elkuldom az uzenetem a fiunak:%s \n",lany);
		msgsnd( uzenetsor, lany, sizeof(lany) , 0 );
		printf("Elkuldve\n"); 
		printf("  Vegeztem (%d)\n",getpid());
		kill(pids[1],SIGUSR1);
	}
	
	return 0;
}