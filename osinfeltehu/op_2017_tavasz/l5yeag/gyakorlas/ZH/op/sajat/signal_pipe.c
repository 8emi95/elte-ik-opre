#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen

void check(int argc){
	if(argc!=2){
		printf("add meg a gyerekeket :v\n");
		exit(1);
	}
}

void handler(int sign){
	printf("Signal '%d' is handled.\n",sign);
}

int main(int argc,char*argv[]){
	check(argc);
	int n = atoi(argv[1]);
	int i;
	char str[64];
	
	pid_t parentID = getpid();
	pid_t pid;
	int pp[2];
	
	if(pipe(pp)==-1){
		perror("pipe shit\n");
		exit(1);
	}
	
	for(i=0;i<n;++i){
		if(parentID == getpid()){
			if((pid=fork())<0) {
				printf("fork shit (%d)",i);
				exit(1);
			}
		}
	}
	printf("      pid -> %d\n",pid);
	if(pid > 0) { // Parent
		printf("Papa itt van (id: %d)\n",getpid());
		int c;
		for(c=1;c<=n;++c) {
			printf(" kell egy signal (%d) :v\n",c);
			signal(SIGUSR1,handler);sleep(1);
			printf(" Megkaptam (%d) :P, olvassunk...\n",c);
			close(pp[1]);
			read(pp[0],str,sizeof(str));
			printf(" ezt olvastam -> %s\n\n",str);			
		}
		int st;
		wait(&st);
		printf(" - %d. varas..\n",c);
		
		printf("Papa vegzett (%d)\n");
	}
	else { // Child
		printf("  | Itt a gyerek (id: %d, par: %d)\n",getpid(),getppid());
		
		printf("  - irok a papanak\n");
		close(pp[0]); sprintf(str,"'ezazID:%d'",getpid());
		write(pp[1],str,strlen(str)+1);
		printf("  - irtam neki (%s)\n",str);
		
		printf("   - uzi papanak (%d)\n",SIGUSR1);
		kill(getppid(),SIGUSR1);
		printf("   - elkuldve (%d)\n",SIGUSR1);
		
		printf("  | Gyerek vege(id: %d, par: %d)\n\n",getpid(),getppid());
	}
	
	return 0;
}