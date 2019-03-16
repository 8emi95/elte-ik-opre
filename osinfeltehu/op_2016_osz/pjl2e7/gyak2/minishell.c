#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    char* str;
    str = (char*)malloc(80*sizeof(char));
    scanf("%s", str);
    char* args[10];
    int j = 0;
    char *command = strtok(str, " ");
    char *p = strtok(str ," ");
    while(p!=NULL){
        args[j++] = p;
        p = strtok(NULL, " ");
    }
    args[j++] = NULL;
    /*int i;
    pid_t child;
    for(i = 0; i < 3; i++){
        scanf("%s", command);
        printf("The command entered: %s \n", command);  
        scanf("%s", args);
        printf("Arguments entered: %s \n", args);  
        child = fork();
        if(child > 0){
            
        }else{
            execv(command, args);
        }
    }*/
    pid_t child = fork();
    if(child > 0){
        
    }else{
        execv(command, args);
    }
    
}