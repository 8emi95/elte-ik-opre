#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
//
// unnamed pipe example
//
int main(int argc, char *argv[])
{
  int parent_to_child[2]; // unnamed pipe file descriptor array
  int child_to_parent[2]; // unnamed pipe file descriptor array
  pid_t pid;
  char sz[100];  // char array for reading from pipe
  if (pipe(parent_to_child) == -1) 
  {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }
  if (pipe(child_to_parent) == -1) 
  {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }

  pid = fork();	// creating parent-child processes
  if (pid == -1) 
  {
    perror("Fork hiba");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) 
  {		    	// child process
    //sleep(3);	// sleeping a few seconds, not necessary
    //close(pipefd[1]);  //Usually we close the unused write end
    close(parent_to_child[0]);
    close(parent_to_child[1]);
    close(child_to_parent[0]);

    printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
    read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
    printf("Gyerek olvasta uzenet: %s",sz);
    printf("\n");
    write(pipefd[1], "Hajra Vasas!", 14);
    close(child_to_parent[1]); // finally we close the used read end
  } 
  else 
  {    // szulo process 
    close(child_to_parent[0]);
    close(child_to_parent[1]);
    close(parent_to_child[0]);
    printf("Szulo indul!\n");
    write(parent_to_child[1], "Hajra Fradi!",13);
    printf("Szulo beirta az adatokat a csobe!\n");
    fflush(NULL); 	// flushes all write buffers (not necessary)
    read(child_to_parent[1], sz, sizeof(sz));
    printf("Ez a valasz %s\n", sz);
    wait();		// waiting for child process (not necessary)
    // try it without wait()
    printf("Szulo befejezte!");	
    close(pipefd[0]);
    close(parent_to_child[1]); // finally we close the used read end
  }
  exit(EXIT_SUCCESS);	// force exit, not necessary
}


