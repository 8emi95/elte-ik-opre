#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 
#include <signal.h>
#include <string.h>
#include "application.h"
#include "event.h"
#define MAXID 32
#define MAXSTRING 128
#define MAXLINE 256

void handler(int sig)
{
	//printf("Handler signal no. is: %i\n",sig);
}

void eventStart(){
	char line[MAXLINE];
	char id[MAXID];
	char hely[MAXSTRING];
    char tomb[MAXSTRING][100];   
    char no_tomb[MAXSTRING][100];
    int no_number;
	
	int i = 1;
	int k;
	int r;
	
	FILE *inputf = fopen("events.txt","r");
    if (inputf == NULL) {
        printf("Error opening file!\n");
        return;
    }
	
	signal(SIGUSR1, handler);
	int pipefd[2];
	pid_t pid;
	char sz1[200];
	int number;
    
	if (pipe(pipefd) == -1) {
		   perror("Hiba a pipe nyitaskor!");
		   exit(EXIT_FAILURE);
		}
		
	while(fgets(line, sizeof(line), inputf) != NULL){
        srand ( time(NULL) );
	    pid = fork();
	    if (pid == -1) {
		   perror("Fork hiba");
		   exit(EXIT_FAILURE);
	    }

	    if (pid == 0) {
			pause();
			read(pipefd[0], sz1, sizeof(sz1));
			printf("Kapott helyszin: %s\n", sz1);
			
			printf("Varakozas a resztvevokre.\n");
			kill(getppid(), SIGUSR1);
            pause();
			
            read(pipefd[0], &number, sizeof(number));
            printf("Resztvevoket megkaptam:\n");
			
            no_number = 0;
            for(k = 0; k < number; k++){
               read(pipefd[0], tomb[k], sizeof(tomb[k]));
               printf("%s\t", tomb[k]);
               r = rand()%100;
               if(r <= 10){
                  stpcpy(no_tomb[no_number], tomb[k]);
                  no_number++;      
               }
               
            }
            printf("\n");
            r = rand()%100+1;
			
            write(pipefd[1], &r,sizeof(r));
            write(pipefd[1], &no_number, sizeof(no_number));
			
            for(k = 0; k < no_number; k++){
				write(pipefd[1], no_tomb[k], sizeof(no_tomb[k]));
            }
            kill(getppid(), SIGUSR1);
			exit(0);
	    } 
	    else 
	    {   

			sleep(1);		
			sscanf(line, "%s\t%s\n", id, hely);                 
			write(pipefd[1], hely, sizeof(hely));
			printf("Esemeny helyszin elkuldve!\n");
			
			fflush(NULL);	
			kill(pid, SIGUSR1);
			pause();
            sleep(1);
			
            number = copyApplicantsWithId(tomb, id);
            write(pipefd[1], &number, sizeof(number));
            for(k = 0; k < number; k++){
                write(pipefd[1], tomb[k], sizeof(tomb[k]));
            }
			
            fflush(NULL);
            printf("Resztvevok elkuldve.\n");
            kill(pid, SIGUSR1);
            pause();
			
            read(pipefd[0], &r,sizeof(r));
            read(pipefd[0], &no_number, sizeof(no_number));
            
            printf("Eredmenyt megkaptam:%d.\n", r);
            printf("Nem erkeztek meg:\n");
            for(k = 0; k < no_number; k++){
               read(pipefd[0], no_tomb[k], sizeof(no_tomb[k]));
               printf("%s\t", no_tomb[k]);
            }  
            printf("\n");            
	    }
        
	}  

    fclose(inputf);
}

int main ()
{   
    srand ( time(NULL) );
    int menu = 0;
    while(menu != '9')
    {
		printf("King of Stands - Keszitette: Laszlo Zsolt (KDL1I4)\n");
        printf("1. Uj vendeg jelentkezes\n");
        printf("2. Vendegek listazasa\n");
        printf("3. Uj rendezveny regisztralasa\n");
        printf("4. Rendezvenyek listazasa\n");
        printf("5. Vendeg adatainak modositasa\n");
		printf("6. Vendeg torlese\n");
		printf("7. Rendezveny torlese\n");
        printf("8. Rendezvenyek lebonyolitasa\n");
		printf("9. Kilepes\n");
        printf("Adja meg a kivant opciot!\n");
        scanf(" %c%*c", &menu);
        switch(menu)
        {
        case '1':
            newApplication();
            break;
        case '2':
            listApplicants();
            break;
        case '3':
            newEvent();
            break;
        case '4':
            listEvents();
            break;			
        case '5':
           editApplication();
            break;
        case '6':
			deleteApplication();
			break;
        case '7':
			deleteEvent();
            break; 
		case '8':
			eventStart();
            break;
		case '9':
            break; 			
        default:
            printf("A kivaslasztott opcio nem letezik, kerem valassza ki ujra!\n");
        }
    }
  return 0;
}





