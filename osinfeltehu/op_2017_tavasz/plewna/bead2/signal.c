#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void handler(int signumber){
  printf("Kerdezobiztos megerkezett\n");
}

int main(){

	int pipe1[2]; // unnamed pipe file descriptor array
	int pipe2[2];
	pid_t pid;
	char sz[100];  // char array for reading from pipe

	if (pipe(pipe1) == -1 || pipe(pipe2) == -1) 
    {
	    perror("Hiba a pipe nyitaskor!");
	    exit(EXIT_FAILURE);
    }
    
    signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                             //handler = SIG_DFL - back to default behavior 
  
    if (pid == -1) 
	{
	    perror("Fork hiba");
	    exit(EXIT_FAILURE);
    }

  pid_t child=fork();
  if (child>0) //parent
  { 
	//várunk hogy megérkezzen a kérdezőbiztos
    pause();
    
	//elküldjük a kérdéseket
	printf("MLSZ kivalasztja a kerdeseket\n");
    close(pipe1[0]); //Usually we close unused read end
    write(pipe1[1], "Hajra Fradi!", 12);
    write(pipe1[1], "Hajra Vasas!", 13);
    int szam = 9;
    write(pipe1[1], &szam , sizeof(int));
    close(pipe1[1]); // Closing write descriptor 
    printf("MLSZ elkuldte a kerdeseket\n");
    fflush(NULL); 	// flushes all write buffers (not necessary)
	
	wait();		
	
	//kiírjuk a konzultáció eredményét
	printf("MLSZ megkapta az eredmenyt\n");
	close(pipe2[1]);  //Usually we close the unused write end
    read(pipe2[0],sz,sizeof(sz)); // reading max 100 chars
    int szam2 = 5;
    read(pipe2[0],&szam2,sizeof(szam2));
    printf("Szülő olvasta uzenet: %s %d",sz, szam2);
    printf("\n");
    close(pipe2[0]); // finally we close the used read end
    printf("Parent process ended\n");
  }
  else //gyerek
  {
	//jelzünk hogy megérkezett a kérdezőbiztos  
    kill(getppid(),SIGTERM); 
	
	//szülő feltölti
	sleep(3);	
	
	//kérdőív beolvasása
	printf("Kerdezobiztos megkapja a kerdeseket\n");
	close(pipe1[1]);  //Usually we close the unused write end
    read(pipe1[0],sz,sizeof(sz)); // reading max 100 chars
    int szam2 = 5;
    read(pipe1[0],&szam2,sizeof(szam2));
    printf("Gyerek olvasta uzenet: %s %d",sz, szam2);
    printf("\n");
    close(pipe1[0]); // finally we close the used read end
	
	//eredmény számítás
	
	//eredmények visszaküldése
	printf("Kerdezobiztos elkuldi az eredmenyt\n");
	close(pipe2[0]); //Usually we close unused read end
    write(pipe2[1], "Hajra Frad!", 11);
    write(pipe2[1], "Hajra Vas!", 11);
    int szam = 9;
    write(pipe2[1], &szam , sizeof(int));
    close(pipe2[1]); // Closing write descriptor
		
    //befejezi a működését
	printf("Kerdezobiztos befejezi a konzultaciot\n");  
  }
  return 0;
}