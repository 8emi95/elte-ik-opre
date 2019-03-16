#include <stdlib.h>  
#include <stdio.h>
#include <sys/types.h> //fork-hoz
#include <sys/stat.h>
#include <unistd.h> //fork
#include <string.h> //strlen
#include <fcntl.h> //lock
#include <signal.h>
#include <sys/time.h>
#include <wait.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

void rendezveny(char fileNev[]){
	
	int pipefd[2];
	pid_t pid;
	char sz[100];
	int i;
	int j;
	
	signal(SIGUSR1,handler);
	
	if (pipe(pipefd) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
	{
		// error
		return;
	}
	else if (pid == 0) 
	{
		char helyszin[100];
		char reszvevok[50][50];
		
		for(i=0;i<50;i++){
			strcpy(reszvevok[i], "blah");
		}
		
		
		sleep(3);
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0],sz,sizeof(sz)); 
		strcpy(helyszin,sz);
		printf("Gyerek olvasta uzenet: %s",sz);
		printf("\n");
		sleep(5);
		kill(getppid(),SIGUSR1);
		
		sleep(6);
		char tmp='i';
		char nev[50];
		printf("A rendezvenyen resztvevok: ");
		for(i=0;tmp!='0';i++){
			read(pipefd[0],(void*)&tmp,sizeof(char));
			for(j=0;tmp!='0'&&tmp!='\n';j++){
				nev[j]=tmp;
				read(pipefd[0],(void*)&tmp,sizeof(char));
			}
			strcpy(reszvevok[i],nev);
			printf("%s \n",nev);
		}
		
		
		sleep(4);
		
		srand(time(NULL));
		int r = rand()%101;
		write(pipefd[1],(void*)&r,sizeof(int));
		
		kill(getppid(),SIGUSR1);
		
		for(i=0;i<50 && reszvevok[i]!="blah";i++){
			r= rand()%10;
			if(r==5){
				write(pipefd[1],reszvevok[i],50);
			}
		}
		write(pipefd[1],"0",10);
		
		close(pipefd[1]); 
		close(pipefd[0]); 
	}
	else 
	{	
		printf("Szulo indul!\n");
		 
		write(pipefd[1], fileNev,30);
		
		printf("Szulo beirta az adatokat a csobe!\n");
		pause();
		FILE* handle_read = fopen(fileNev, "r");
		if (handle_read == 0)
			{
				printf("File error.\n");
				return;
			}
		//char* buffer = malloc(sizeof(char)*(strlen(data)+1));
		//fgets(buffer, strlen(data)+1, handle_read);
		char ch;
		ch=fgetc(handle_read);
		while(ch!=EOF){
			ch=fgetc(handle_read);
			write(pipefd[1],(void*)&ch,sizeof(char));
		}
		write(pipefd[1],"0",10);
		//fgets(buffer, 50, handle_read);
		fclose(handle_read);
		
		pause();
		read(pipefd[0],(void*)&ch,sizeof(ch));
		printf("A rendezveny %i szazalekban sikeres volt!\n",ch);
		
		printf("Nem jelentek meg a rendezvenyen:\n");
		while(sz!="0"){
			read(pipefd[0],sz,50);
			printf("%s \n",sz);
		}
		
		fflush(NULL); 
		wait(NULL);		
		printf("Szulo befejezte!");	
		close(pipefd[0]);
		close(pipefd[1]); 
	}	
}

int main(){
	FILE* myfile = fopen("rendezvenyek", "r");
	if (myfile == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	char filenev[30];
	char ch;
	int tmp=0;
	
	ch=getc(myfile);
	while(ch!=EOF){
		while(ch!='\n' && ch!=EOF){
			filenev[tmp]=ch;
			tmp++;
			ch=getc(myfile);
		}
		if(ch=='\n'){
			rendezveny(filenev);
			ch=getc(myfile);
		}
		sleep(10);
		memset(&filenev[0], 0, sizeof(filenev));
		tmp=0;
	}
	
	
	fclose(myfile);
	

  return 0;
}