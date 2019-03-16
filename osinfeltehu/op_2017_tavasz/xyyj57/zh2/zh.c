#include <string.h>
#include<stdio.h> 
#include<unistd.h> 
#include <stdlib.h>

#define BUFF_SIZE 1024



int main(int argc, const char* argv[]) { 
	
	if(argc == 0){
		printf("Bad args");
		return 0;
	}
    int fd[2];  
    int child;
	char input[BUFF_SIZE];

	stpcpy(input, argv[1]);

    if( pipe(fd) ) { 
        perror("pipe"); 
        return 1; 
    }
    
    child = fork(); 
    
    if(child > 0) { /*KOZPONT*/
        char s[BUFF_SIZE]; 
        close(fd[1]); 
        read(fd[0], s, sizeof(s)); 
        printf("Utas cime: %s\n", s); 
        
		int returnStatus;    
		waitpid(child, &returnStatus, 0);  // Parent process waits here for child to terminate.

		if (returnStatus == 0)  // Verify child process terminated without error.  
		{
			printf("Utas folyamat rendben.\n");  
		}

		if (returnStatus == 1)      
		{
			printf("Utas folyamat hiba.\n");   
		}
		
		printf("Kozpont bezar.\n}");
		close(fd[0]);
        
    } else if(child == 0) { /*UTAS*/
    
        close(fd[0]);
		
        write(fd[1], input, BUFF_SIZE); 
		
        close(fd[1]); 
    } 
    
    return 0; 
}