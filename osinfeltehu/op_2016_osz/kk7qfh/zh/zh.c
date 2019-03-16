#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdbool.h>



#define clear() printf("\033[H\033[J")


void menu();
void hozzadas();
void listazas();
void modositas();
void vendTorles();
void ujRendezveny();
void rendTorles();
void lebonyolitas();

int main(int argc, char *argv[])
{
	printf("Budapest Metro Funs!\n Nem jo ez a verza!!!\n");
	int max = atoi(argv[1]);
	//printf("%s%d\n", argv[1], max);
	

	//pipe
	int pipefd[5]; // unnamed pipe file descriptor array
    pid_t pid;
    char sz[200];  // char array for reading from pipe
    char sz2[200];  // char array for reading from pipe
	int a[2] = {25, 30};
	int n, m=0;
	key_t kulcs;
	kulcs = ftok("/tmp",'a'); 
	int uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 

    if (pipe(pipefd) == -1){
		perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
    pid = fork();	// creating parent-child processes
	
    if (pid == -1){
		perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){		// child process
	
			sleep(1); 
		    close(pipefd[1]);  //Usually we close the unused write end
			
			
			printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
			int i;
			/*n = read(pipefd[0], sz, sizeof(sz));
			n = read(pipefd[0], sz2, sizeof(sz2));
			printf("Gyerek irja: En vagyok ma a(z) %s. szerelveny.\n",sz2);	
			int max2 = atoi(sz);
			int max3 = atoi(sz2);
			printf("Gyerek irja: Meg lesz ma %idb utanam.\n",max2-max3);	
			
			*/
			
			int status;
			status = msgrcv(uzenetsor, &sz, 1024, 0, 0 );
			if ( status < 0 )
			{
				perror("msgrcv"); 
			}
			//printf( "Gyerek irja: En vagyok ma a(z) %s. szerelveny.\n",sz);
			
			int statuss;
			statuss = msgrcv(uzenetsor, &sz2, 1024, 0, 0 );
			if ( statuss < 0 )
			{
				perror("msgrcv"); 
			}
			
			printf("Gyerek irja: En vagyok ma a(z) %s. szerelveny.\n",sz2);	
			int max2 = atoi(sz);
			int max3 = atoi(sz2);
			printf("Gyerek irja: Meg lesz ma %idb utanam.\n",max2-max3);	
			
			int r=(rand()%100)+1;
			srand(time(NULL));
			sleep(1);
			r=(rand()%100)+1;
			//printf("%i%s\n",r,"");
			char str2[15];
		
			if(r<=10){
				
				printf("Sajnos elcsusztam :(\n");
				strcpy(str2, "Sajnos elcsusztam :(\n");
			}else{
				printf("Sikeresen teljesitettem!\n");
				strcpy(str2, "Sikeresen teljesitettem!\n");
			}
			
			//printf( "Gyerek irja: En vagyok ma a(z) %s. szerelveny.\n",sz2);	
			
			
			
			int statusss;
			statusss = msgsnd( uzenetsor, &str2, sizeof(str2), 0 );
			if ( statusss < 0 )
			{
				perror("msgsnd"); 
			}
									
				
               close(pipefd[0]); // finally we close the used read end
			   fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();	
	
	
	}else {    // szulo process 
              
			  printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
			   
			   int status;
			   
				strcpy(sz, argv[1]);
				//printf("debug %s\n", sz);
				
				status = msgsnd( uzenetsor, &sz, sizeof(sz), 0 );
				if ( status < 0 )
				{
					perror("msgsnd"); 
				}
				
				//write(pipefd[1], sz,sizeof(sz));
               
				int r=(rand()%max)+1;
				srand(time(NULL));
				sleep(1);
				r=(rand()%max)+1;
				//printf("%i%s\n",r,"");
				
				char str[15];
				sprintf(str, "%d", r);
				
				//write(pipefd[1], str,sizeof(str));
				int statuss;
				statuss = msgsnd( uzenetsor, &str, sizeof(str), 0 );
				if ( statuss < 0 )
				{
					perror("msgsnd"); 
				}
				
				//printf("%d%s\n", count, " ember nem erkezett meg!");
				char str2[15];
				int statusss;
					statusss = msgrcv(uzenetsor, &str2, 1024, 0, 0 );
					if ( statusss < 0 )
					{
						perror("msgrcv"); 
					}
					
					
				printf("Gyerek irja: En vagyok ma a(z) %s. szerelveny.\n",str2);
				
				/*FILE *fp;
				int ch = 0;
				int lines = 0;

				//fp = fopen("vendegek.txt", "r");
						
				
				char *line = NULL;
				size_t len = 0;
				char read;
				

				if (fp == NULL){
					printf("hiba");
				}
				int seged;
				int j;
				char * pch;

				while ((read = getline(&line, &len, fp)) != -1) {
					for(j=0; j<5;j++){
						if(line[i]==';'){
							break;
							
						}else{
							
						}
					}
					
					if((int)line[0]-'0' == (int)i){
						
							pch = strtok (line,";");
							while (pch != NULL)
							{
								write(pipefd[1], pch,sizeof(sz));
								pch = strtok (NULL, ";");
							}
						
					}
					
					
				}

				if (line){
					free(line);
				}
					
				fclose(fp);
				printf("\n");*/
				
			   
			   
                
				close(pipefd[1]); // Closing write descriptor 
               
			   
				/*int status;
						
				status = msgrcv(uzenetsor, &sz2, 1024, 0, 0 );
				if ( status < 0 )
				{
					perror("msgrcv"); 
				}
				printf( "Szulo kapja: %s\n", sz2 );
						
				printf("Szulo beirta az adatokat a csobe!\n");
				
				*/
						
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
			   
			   
				// try it without wait()
	       printf("Szulo befejezte!\n");	
	   }

	return 0;
}

void menu() {

	int i;
	
	FILE *fp;

	
	

	
	printf("Menu:\n");
	printf("1. Uj rendezveny inditasa.\n");
	printf("2. Rendezveny torlese.\n");
	printf("3. Esemeny lebonyolitasa\n");
	printf("4. Kilepes\n\n");
	printf("Valassz menupontot: ");
	scanf("%d", &i);

	switch (i){

		
		case 1:
			clear();
			ujRendezveny();
			menu();
			break;

		case 2:
			clear();
			rendTorles();
			menu();
			break;
		
		case 3:
			clear();
			lebonyolitas();
			menu();
			break;
		
		
		case 4:
			exit(0);
			break;
		

		default:
			printf("Helytelen menupont lett megadva! Probald ujra!\n\n");
			menu();
			break;
	}




}




void ujRendezveny() {

	FILE *fp;
	char nev[80];
	char azon[80];

	printf("\nUj rendezveny inditasa:\n");
	printf("Rendezveny azonositoja: ");
	scanf("%79s", azon);
	printf("Rendezveny neve: ");
	scanf("%79s", nev);
	fp = fopen("rendezvenyek.txt", "a+");
	fprintf(fp, "%s;%s\n", azon, nev);
	fclose(fp);

}

void rendTorles() {
	
	int seged = 0;
	char nev[80];
	char buf[512];
	char *p;

	FILE *fileptr1, *fileptr2;
	char filename[] = "rendezvenyek.txt";
	char ch;
	int delete_line = 1;
	int temp = 1;

	
	printf("Add meg a torlendo rendezveny nevet: ");
	scanf("%79s", nev);


	fileptr1 = fopen(filename, "r");
	ch = getc(fileptr1);

	while (fgets(buf, 512, fileptr1) != NULL) {
		if ((strstr(buf, nev)) != NULL) {
			
			break;
		}
		delete_line++;
	}
	//rewind
	rewind(fileptr1);
	
	fileptr2 = fopen("tmp.txt", "w+");
	char line[1024]; 
	int lineCount = 0;
	while (fgets(line, sizeof(line), fileptr1) != NULL)
	{
		if (lineCount != delete_line -1)
		{
			fprintf(fileptr2, "%s", line);
		}

		lineCount++;
	}

	fclose(fileptr1);
	fclose(fileptr2);
	remove(filename);
	rename("tmp.txt", filename);



}

void lebonyolitas(){
	
	int i;
	
	FILE *fp;
	int ch = 0;
	int lines = 0;
	
	printf("Esemeny lebonyolitasa: \n");
	fp = fopen("rendezvenyek.txt", "r");
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch != ';') {
			putchar(ch);
		}
		else {
			printf(" - ");
		}
		
	}
	fclose(fp);
	printf("\n");

	scanf("%d", &i);

	//pipe
	int pipefd[5]; // unnamed pipe file descriptor array
    pid_t pid;
    char sz[200];  // char array for reading from pipe
    char sz2[200];  // char array for reading from pipe
	int a[2] = {25, 30};
	int n, m=0;
	key_t kulcs;
	kulcs = ftok("/tmp",'q'); 
	int uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 

    if (pipe(pipefd) == -1){
		perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
	
    pid = fork();	// creating parent-child processes
	
    if (pid == -1){
		perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){		// child process
	
			sleep(1); 
		    close(pipefd[1]);  //Usually we close the unused write end
			
			strcpy(sz2, "Helyszinen vagyok.");
			int status;
			status = msgsnd( uzenetsor, &sz2, sizeof(sz2), 0 );

			if ( status < 0 )
			{
				perror("msgsnd"); 
			}
			
			printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               while ((n = read(pipefd[0], sz, sizeof(sz))) > 0) 
						{
							printf("Gyerek kapta: %s",sz);
							m = m + 1;
							printf("\n");
							
						}
				m = m-1;
				m = m/4;
				
				int count=0, k=0;
				int r=(rand()%100)+1;
				printf("%s%i%s\n","Rendezveny sikeressege: ", r, "%");
				srand(time(NULL));
				for(k=0; k<m; k++){
					sleep(1);
					r=(rand()%10);
					printf("%i%s\n",r,"0%");
					if(r==0){
						
						count = count+1;
					}
					
				}
				
				printf("%d%s\n", count, " ember nem erkezett meg!");
									
				
               close(pipefd[0]); // finally we close the used read end
			   fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();	
	
	
	}else {    // szulo process 
              
			  printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
			   
				strcpy(sz, "Ok, bonyolitsd le az esemenyt!");
				write(pipefd[1], sz,sizeof(sz));
               
				FILE *fp;
				int ch = 0;
				int lines = 0;
				int count;

				fp = fopen("vendegek.txt", "r");
						
				
				char *line = NULL;
				size_t len = 0;
				char read;
				

				if (fp == NULL){
					printf("hiba");
				}
				int seged;
				int j;
				char * pch;

				while ((read = getline(&line, &len, fp)) != -1) {
					for(j=0; j<5;j++){
						if(line[i]==';'){
							break;
							
						}else{
							
						}
					}
					
					if((int)line[0]-'0' == (int)i){
						
							pch = strtok (line,";");
							while (pch != NULL)
							{
								write(pipefd[1], pch,sizeof(sz));
								pch = strtok (NULL, ";");
							}
						
					}
					
					
				}

				if (line){
					free(line);
				}
					
				fclose(fp);
				printf("\n");
				
			   
			   
                
				close(pipefd[1]); // Closing write descriptor 
               
			   
				int status;
						
				status = msgrcv(uzenetsor, &sz2, 1024, 0, 0 );
				if ( status < 0 )
				{
					perror("msgrcv"); 
				}
				printf( "Szulo kapja: %s\n", sz2 );
						
				printf("Szulo beirta az adatokat a csobe!\n");
				
				
						
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
			   
			   
				// try it without wait()
	       printf("Szulo befejezte!");	
	   }
	
}

