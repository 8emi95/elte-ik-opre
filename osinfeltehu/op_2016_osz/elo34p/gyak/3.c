#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int main(){
	
	//pid_t típus process ID-k(igazából egy int(sorszám))
	
	/* You can get the process ID of a process by calling getpid.
	The function getppid returns the process ID of the parent of the current process
	(this is also known as the parent process ID).
	Your program should include the header files `unistd.h' and `sys/types.h' to use these functions.
	*/
	
	/* - pid_t getpid (void)
			The getpid function returns the process ID of the current process.
	   - pid_t getppid (void)
			The getppid function returns the process ID of the parent of the current process.
	*/
	
	/*System call fork() is used to create processes. It takes no arguments and returns a process ID.
	The purpose of fork() is to create a new process, which becomes the child process of the caller.
	After a new child process is created, both processes will execute the next instruction following the fork()
	system call. Therefore, we have to distinguish the parent from the child.
	This can be done by testing the returned value of fork():
		If  1, fork() returns a negative value, the creation of a child process was unsuccessful.
			2, fork() returns a zero to the newly created child process.
			3, fork() returns a positive value, the process ID of the child process, to the parent.
			   The returned process ID is of type pid_t defined in sys/types.h. Normally, the process ID is
			   an integer. Moreover, a process can use function getpid() to retrieve the process ID assigned to this process.*/
	
	int status;
	
	pid_t child=fork();
	pid_t child2=fork();
	
	//child=getpid();
	
	if(child<0){
		perror("The fork calling was not succesful\n");
		exit(1);
	}
	else if(child2<0){
		perror("The fork calling was not succesful\n");
		exit(1);
	}
	
	else if(child>0){
		waitpid(child,&status,0); 
		printf("Gyerek vagyok!\n");
	}
	else if(child2>0){
		waitpid(child2,&status,0); 
		printf("Gyerek2 vagyok!\n");
	}
	else{
		printf("Nem tudom.\n");
		child=getpid();
		printf("get pID %i\n",child);
		child=getppid();
		printf("get ppID %i\n",child);
		child2=getpid();
		printf("get pID %i\n",child2);
		child2=getppid();
		printf("get ppID %i\n",child2);
	}
	
	return 0;
	
}