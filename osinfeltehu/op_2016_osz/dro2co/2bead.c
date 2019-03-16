//Vitányi Anna, DRO2CO
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

int randomszam();
void readStringPacket(int, char*);

int main(){
	int pipefd[2]; //pipehoz tömb
	int pipech[2];
	pid_t pid; //gyerek - 0 /nemgyerek 
	char sz[100]; //ez megy a pipeban
	srand(time(NULL));

	if (pipe(pipefd) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    } 

    if (pipe(pipech) == -1) {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    } 

    pid = fork();	// ha nulla akor gyerek, negatív hiba, más szülő
   if (pid == -1) {
       perror("Fork hiba");
       exit(EXIT_FAILURE);
   }

   if (pid == 0) {
   		char helyszin[100];

      readStringPacket(pipefd[0], helyszin);

   		printf("gyerek: %s\n", helyszin);

		printf("gyerek: gyerek utazik :) \n");
		usleep(50000); 
		write(pipech[1], "gyerek odaert|", 14);

    char nevek[10][100];
    int meret;
    int i;
    char meretbuff[2];


    readStringPacket(pipefd[0], meretbuff);
    meret = atoi(meretbuff);

    for(i=0; i< meret; ++i){
      readStringPacket(pipefd[0], nevek[i]);
    }


    for(i = 0; i < meret; i++) {
      printf("Nevek: %s\n", nevek[i]);
    }



		int r = randomszam();
		char str[80];
   	sprintf(str, "A rendezveny sikeressege: %i|", r);
		write(pipech[1], str, strlen(str));

    for(i=0; i<meret; i++){
      if (randomszam() < 90){
        write(pipech[1], nevek[i], strlen(nevek[i]));
        write(pipech[1], "|", 1);
      }
    }
    write(pipech[1], "|", 1);    


    close(pipefd[0]);
    close(pipefd[1]);
    close(pipech[0]);
    close(pipech[1]);
   } else { //szülő
   		write(pipefd[1], "Valetta|", 8);

   		readStringPacket(pipech[0], sz);
   		printf("szulo: %s\n", sz);

      write(pipefd[1], "3|", 2);
   		write(pipefd[1], "Jozsi 1.0|", 10);
   		write(pipefd[1], "Jozsi 2.0|", 10);
   		write(pipefd[1], "Jozsi 3.0|", 10);

      readStringPacket(pipech[0], sz);
      printf("szulo: %s\n", sz);

      do{
        readStringPacket(pipech[0], sz);
        if (strlen(sz) == 0) {
          break;
        }
        printf("%s\n", sz);
      }while(1);

   		close(pipefd[0]);
   		close(pipefd[1]);
   		close(pipech[0]);
    	close(pipech[1]);
   		wait(NULL); //megvarja a gyereket
   }



}

void readStringPacket(int p, char* buff) {
  char c;
  int offset = 0;
  while(read(p, &c, 1) > 0){
      if (c != '|' ) {
        buff[offset] = c;
        offset++;
      } else {
        buff[offset] = '\0';
        break;
      }
    }
}


int randomszam(){
  int r=rand()%100; 
  return r+1;
}
