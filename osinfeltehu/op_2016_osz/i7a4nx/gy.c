#include <stdio.h>
#include <stdlib.h>

//volatile
//fflush(stdout)

void elnok(){
	
}

void pecsetelo(){
	
}

void ellenorzo(){
	kill(getppid(),SIGUSR1);	//munkara kesz
	//...
}

int main(int argc,char** argv) 
{
  int i, szavazatokszama;
  signal(SIGUSR1,handler);
  
  if(argc!=2) {perror("keves argumentum");}
  szavazatokszama=atoi(argv[1]);
  
  if(pipe(fd)<0){perror("Hibas a cso letrehozas\n");exit(EXIT_FAILURE);}
  
  pid_t  ellen=fork();
      
	 if (ellen>0) 
	 {
		pid_t pecsetelo=fork();
		
		if (pecsetelo>0){
			while(szamlalo!=2){;}	//pause helyett
			elnok();
		
		wait(NULL); wait(NULL);
		//waitpid(child,&status,0);  
		}
		else {
			pecset();
		}

	}
	else{
		ellenorzo();
	}

	
 return 0;
}