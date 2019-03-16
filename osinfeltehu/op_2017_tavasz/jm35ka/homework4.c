#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>


int main(){

	int status,f;
	int i,j=0;
	//int value=5;

	struct flock fl;

	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start=0;
	fl.l_len=0;
	fl.l_pid = getpid();
	
	char text1[50]="This is text 1.\n";
	char text2[50]="Another one here for the parent.\n";
	
	f=open("hw4file.txt", O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	fcntl(f,F_SETLKW, &fl);

	//printf("Initial value: %d\n", value);
	//++value;
	//printf("Value of integer in child process: %d\n", value);	
	//printf("Value of integer in parent process: %d\n", value);
	
	pid_t process=fork();

	for(i=0; i<25; i++){
		
			for(j=0; j<50; j++){
				
				if(process==0) {
					fl.l_type = F_UNLCK;
					fcntl(f, F_SETLK, &fl);
					write(f, &text1[j], sizeof(text1[j])); }

				else {write(f, &text2[j], sizeof(text2[j]));}
				
			
		} 
	}
	
	
//	wait(&status);
	
	return 0;
}
