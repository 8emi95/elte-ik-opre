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
	if(sign==SIGUSR2);
}

int main(){
	pid_t parentID = getpid();
	pid_t pid, pids[gyerekek];
	int pp[2],i;
	char str[2];
		
	signal(SIGPOLL, handler);
	signal(SIGTERM, handler);
	signal(SIGUSR1,handler);
	
	if(pipe(pp)==-1){
		perror("pipe fail\n");
		exit(1);
	}
	
	for(i=0;i<gyerekek;++i){
		if(parentID == getpid()){
			pid=fork();
			if(pid<0) {
				printf("fork fail (%d)",i);
				exit(1);
			}
			pids[i]=pid;
		}
	}
	if(pid > 0) { // Parent
		srand(time(NULL));
		kezdo = rand()%2;
		
				
		for(i=0;i<gyerekek;++i){
			pause();
			printf("Megkaptam egyik jelzest..\n");
		}
		
		for(i=0;i<gyerekek;++i){
			sprintf(str,"%d",kezdo+1);
			close(pp[0]);
			write(pp[1],str,strlen(str)+1);
			kill(pids[i],SIGUSR1);
		}
		
		printf("A kezdojatekos a %d. szamu\n",kezdo+1);
		
		for(i=0;i<gyerekek;++i){
			int st;
			wait(&st);
		}
		printf("Vege a sakknak\n");
	}
	else { // Child
		printf("  Itt vagyok, jelzek vissza (%d)\n",getpid());
		kill(getppid(),SIGUSR1);
		
		printf("\n  Most varok a kezdesre..(%d)\n",getpid());
		pause();
		
		close(pp[1]);
		read(pp[0],str,sizeof(str));
		
		printf("  Szoval a(z) %s. kezd..(%d)\n",str,getpid());	
		if((getpid()+(atoi(str)+1))%2 == 0)
			printf("   En kezdek! (%d)\n",getpid());
		else
			printf("   Nem en kezdek! (%d)\n",getpid());
				
		printf("  Vegeztem (%d)\n",getpid());
	}
	
	return 0;
}