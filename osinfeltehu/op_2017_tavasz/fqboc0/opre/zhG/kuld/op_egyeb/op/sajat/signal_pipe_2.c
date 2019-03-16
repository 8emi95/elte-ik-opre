#include <stdio.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <string.h> //strlen
#include <sys/time.h> //time

void check(int argc){
	if(argc!=2){
		printf("add meg a gyerekeket :v\n");
		exit(1);
	}
}

void handler(int sign){
	printf("");
	//printf("Signal '%d' is handled.\n",sign);
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
	if(pid > 0) { // Parent
		printf("Papa itt van (id: %d)\n",getpid());
		int c;
		for(c=1;c<=n;++c) {
			signal(SIGUSR1,handler);sleep(1);
			close(pp[1]);
			read(pp[0],str,sizeof(str));
			printf(" ezt olvastam -> %s\n",str);			
		}
		int st;
		wait(&st);
		
		printf("Papa vegzett (%d)\n",getpid());
	}
	else { // Child
		srand(time(NULL)^(getpid()<<1));
		int list[4];
		for(i=0;i<4;++i)
			list[i] = rand()%100;
		
		sprintf(str,"'%d",getpid());
		for(i=0;i<4;++i) sprintf(str,"%s,%d",str,list[i]);
		sprintf(str,"%s'",str);
		
		printf("  |gyerek -> %s | papa -> %d\n",str,getppid());
		
		close(pp[0]);
		write(pp[1],str,strlen(str)+1);
		
		kill(getppid(),SIGUSR1);
		printf("  | Volt gyerek (id: %d, par: %d)\n\n",getpid(),getppid());
	}
	
	return 0;
}