#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 

void handler(int signumber){
  printf("Van egy utasunk\n",signumber);
}


int main(int argc, char *argv[])
{
 int status;
 int namedpipe=mkfifo("fifo.asd", S_IRUSR|S_IWUSR);
 int fd;
 
signal(SIGRTMIN,handler); 

 pid_t  child=fork(); 
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process
 {
    pause();    
    pid_t taxi=fork();
    if(taxi>0){
        //printf("szulo");
        fd=open("fifo.asd", O_RDONLY);
        char readpipe[512] ="Valami";
        read(fd,readpipe,512);
        printf("A lakcím: %s \n", readpipe);
        close(fd);
        wait(NULL);
        unlink("fifo.asd");
    }
    else {        
        if (argc > 1) {
            fd=open("fifo.asd", O_RDWR);
            write(fd,argv[1],sizeof(argv[1]));
            close(fd);
        } else {
            printf ("Nincs megadva lakcim!");
            exit(1);
        }        
    }
    
 }
 else //child process
 {
    kill(getppid(),SIGRTMIN);    
    
 }
 return 0;
}