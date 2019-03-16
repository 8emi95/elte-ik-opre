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
    //close(parent_to_child[0]);
    close(child_to_parent[0]);
    close(parent_to_child[1]);

    printf("Gyerek: indulok!\n");
    printf("Gyerek: adatokra varok!\n");
    read(parent_to_child[0],sz,sizeof(sz)); // reading max 100 chars
    printf("Gyerek: adatot kaptam: %s\n",sz);
    printf("Gyerek: elso valaszomat kuldom: Hajra Vasas!\n");
    write(child_to_parent[1], "Hajra Vasas!", 14);
    printf("Gyerek: elkuldtem, szulo megkapta!\n");
    close(child_to_parent[1]); // finally we close the used write end
    close(parent_to_child[0]); // finally we close the used read end
  } 
  else 
  {    // szulo process 
    //close(child_to_parent[0]);
    close(child_to_parent[1]);
    close(parent_to_child[0]);
    printf("Szulo: indulok!\n");
    printf("Szulo: kezdem a kommunikaciot ezzel: Hajra Fradi!\n");
    write(parent_to_child[1], "Hajra Fradi!",13);
    printf("Szulo: beirtam az adatokat a csobe, gyerek olvasta is!\n");
    fflush(NULL); 	// flushes all write buffers (not necessary)
    read(child_to_parent[0], sz, sizeof(sz));
    printf("Szulo: ez a gyerek valasza: %s\n", sz);
    printf("Szulo: varok a gyerekemre!\n");	
    wait();		// waiting for child process (not necessary)
    // try it without wait()
    printf("Szulo: befejeztem!\n");	
    close(child_to_parent[0]); // finally we close the used read end
    close(parent_to_child[1]); // finally we close the used write end
  }
  exit(EXIT_SUCCESS);	// force exit, not necessary
}


