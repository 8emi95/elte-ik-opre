#include <stdlib.h>  
#include <stdio.h>
#include <sys/types.h> //fork-hoz
#include <sys/stat.h>
#include <unistd.h> //fork
#include <string.h> //strlen
#include <fcntl.h> //lock

int main(){
	
	int f;
	f=open("mydata.txt", O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);

	struct flock data_lock;
	data_lock.l_whence=SEEK_SET;
	data_lock.l_start=0;
	data_lock.l_len=0;
	int rc=0;

	pid_t child = fork();

	if(child < 0){
		perror("Error");
		exit(1);
	}
	if(child>0){
		//parrent process
		printf("process %d is locking\n", (int) getpid());
		data_lock.l_pid = getpid();
		data_lock.l_type = F_WRLCK;
		rc = fcntl(f, F_SETLKW, &data_lock);
		if(rc != (-1)){
		int j;
		for(j=0; j<25; j++){
			write(f, "Parent ", 1);
			usleep(20);
		}
		write(f, "\n", 1);
		data_lock.l_type=F_UNLCK;
		fcntl(f, F_SETLKW, &data_lock);
	}
	int status;
	waitpid(child, &status, 0); //wait for the end of the child proccess
	}else{
		//child
		printf("child process %d is locking\n", (int) getpid());
		data_lock.l_pid = getpid();
		data_lock.l_type = F_WRLCK;
		rc = fcntl(f, F_SETLKW, &data_lock);
		if(rc != (-1)){
		int j;
		for(j=0; j<25; j++){
			write(f, "Child ", 1);
			usleep(20);
		}
		write(f, "\n", 1);
		data_lock.l_type=F_UNLCK;
		fcntl(f, F_SETLKW, &data_lock);

	}
	close(f);

	return 0;
}
}