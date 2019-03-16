#include <stdio.h>
#include <unistd.h> //fork, pipe
#include <sys/wait.h> //waitpid
#include <stdlib.h> //exit
#include <sys/time.h> //time
#include <string.h> //strlen

#define ADATOK 4

void handler(int s){
	printf("  Megjott a signal(%d)\n",s);
}

int main(int argc, char* argv[]){
	if(argc!=2){
		printf("add meg, mennyi gyerek legyen :v\n");
		exit(1);
	}
	int n=atoi(argv[1]);
		
	char str[64];
	
	signal(SIGTERM,handler);
	
	int pp[ADATOK+1][2],i;
	for(i=0;i<ADATOK+1;++i)
		if(pipe(pp[i]) == -1) {perror("Pipe fail!\n");exit(1);}
	
	pid_t child,children[n];
	pid_t parentID = getpid();
	
	for(i=0;i<n;++i){
		if(parentID == getpid()){
			child = fork();
			children[i] = child;
			if(child < 0) {perror("Fork failed\n");exit(1);}
		}
	}
	
	if(child > 0) {
		int st,check=0;
		while(check!=n){
			wait(&st);
			++check;
			printf("\n ~~~ Olvasas ~~~\n");
			
			printf("Olvasok a csobol..\n");
			
			close(pp[0][1]);
			read(pp[0][0],str,sizeof(str));
			close(pp[0][0]);
			printf("Tole: %s\n",str);
			
			for(i=0;i<ADATOK;++i) {
				close(pp[i+1][1]);
				read(pp[i+1][0],str,sizeof(str));
				close(pp[i+1][0]);
				printf("Ezt olvastam -> %s\n",str);
			}
		}
		
		
		printf("Ennyi volt, nincs lufi.\n");
	}
	else {
		printf("\n--- Lufi adatai ---\n");
		srand(time(NULL)^(getpid()<<1));
		int lufi[ADATOK],i;
		for(i=0;i<ADATOK;++i){
			lufi[i]=rand()%100;
		}
		printf("Az en adataim: %i, %i, %i, sugar: %i\n",
			lufi[0],lufi[1],lufi[2],lufi[3]);
			
		
		printf(" Irok a csobe... (%i)\n", getpid());
		sprintf(str,"%i",getpid());
		close(pp[0][0]);
		write(pp[0][1],str,strlen(str)+1);
		close(pp[0][1]);
		
		for(i=0;i<ADATOK;++i) {
			sprintf(str,"adat %i, %i",i,lufi[i]);
			close(pp[i+1][0]);
			write(pp[i+1][1],str,strlen(str)+1);
			close(pp[i+1][1]);
			printf("  Ezt irtam be -> %s\n",str);
		}
		//printf(" Signal vissza (%d)..\n", SIGTERM);
		//kill(getppid(),SIGTERM);
		//sleep(1);
				
		printf(" A lufi kipukkant..\n\n");
	}
	
	
	return 0;
}