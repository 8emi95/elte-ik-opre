#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen
#include <sys/time.h> //time
#include <signal.h> //sigval

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
	
	pipe(p1);
	pipe(p2);
	int i;
	pid=fork();
	pids[0]=pid;
	if(pid > 0) { // Parent
		pid=fork();
		if(pid>0){//parent

			srand(time(NULL));
			
					
			for(i=0;i<gyerekek;++i){
				pause();
				printf("Megkaptam egyik jelzest..\n");
			}
			int szavazok;
			char in[512];
			printf("Kerem a szavazok szamat: \n");
			scanf("%s",in);
			szavazok=atoi(in);
			int azonosito;
			char azonositok[512];
			strcpy(azonositok,"");
			for(i=0; i<szavazok; ++i){
				azonosito=rand();
				char text[80];
				printf("A generalt azonosito: %d\n",azonosito);
				snprintf(text, sizeof(text),"%d, ",azonosito);
				strcat(str, text);
			}
			close(p1[0]);
			write(p1[1],str,sizeof(str));

			kill(pids[0],SIGUSR1);
			sleep(3);
			kill(pids[1],SIGUSR1);

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
		}else{//child2
			printf("  Itt vagyok, jelzek vissza (%d)\n",getpid());
			kill(getppid(),SIGUSR1);
			printf("  Most varok a kezdesre..(%d)\n",getpid());
			pause();
			printf("En vegeztem is!\n");
		}
		
	}
	else { // Child1
		printf("  Itt vagyok, jelzek vissza (%d)\n",getpid());
		kill(getppid(),SIGUSR1);
		
		printf("  Most varok a kezdesre..(%d)\n",getpid());
		pause();
		char azonositok[512];
		close(p1[1]);
		read(p1[0],str,sizeof(str));
		printf("Fogadtam az adatokat!\n");
		printf("%s\n", str);
		printf("  Vegeztem (%d)\n",getpid());
	}
	
	return 0;
}