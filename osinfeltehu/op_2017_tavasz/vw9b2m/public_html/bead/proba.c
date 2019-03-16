#include <stdlib.h>  
#include <stdio.h>

#include <sys/stat.h>

#include <string.h> //strlen
#include <fcntl.h> //lock
#include <errno.h> //perror, errno
#include <sys/types.h> //open
#include <sys/stat.h>
#include <time.h>


#define MAX_INPUT 256

struct question_body{
	char theQuestion[MAX_INPUT];
	char answer_1[MAX_INPUT];
	char answer_2[MAX_INPUT];
	char answer_3[MAX_INPUT];
	time_t recordDate;
};

void newQuestion(struct question_body new);
void listQuestions();
const char * ido_formatum(time_t t);
void deleteByNumber(int sorszam);
void editByNumber(int sorszam);
void finalQ(int sorszam);

int main(int argc,char** argv)
{
	char answer[1];
	
	do{
		printf("\n**********************************\n");
		printf("Uj kerdes: 1\n");
		printf("Kerdes modositasa: 2\n");
		printf("Kerdes torlese: 3\n");
		printf("Kerdesek listazasa: 4\n");
		printf("Kerdoiv veglegesitese: 5\n");
		printf("Kilepes: X\n");
		printf("**********************************\n");

		scanf("%c", answer);
		char dummy[MAX_INPUT];
		fgets(dummy, MAX_INPUT, stdin);
		
		if(strcmp(answer, "1")==0){
			//printf("Uj kerdest szeretne felvenni.\n");
			fflush(stdin);
			struct question_body new;
			printf("Kerdes:\n");

			fgets(&new.theQuestion, MAX_INPUT, stdin);

			printf("Elso valasz:\n");
			fgets(&new.answer_1, MAX_INPUT, stdin);
			
			printf("Masodik valasz:\n");
			fgets(&new.answer_2, MAX_INPUT, stdin);
			
			printf("Harmadik valasz:\n");
			fgets(&new.answer_3, MAX_INPUT, stdin);
		
			new.recordDate = time(NULL);
			
			newQuestion(new);
		}
		else if(strcmp(answer, "2")==0){
			//printf("Kerdest akar modositani.\n");
			char dummy2[MAX_INPUT];
			
			printf("-- A jelenlegi felvitt kerdesek listaja --");
			printf("\n------------------------------------------\n");
			listQuestions();
			printf("\n------------------------------------------\n");
			printf("Melyik kerdest akarja modositani?\n");
			
			char sorszam = 0;
			
			scanf("%d", &sorszam);
			fgets(dummy2, MAX_INPUT, stdin);
			//if(sorszam == ) tudjon visszalepni ha akar
			editByNumber(sorszam);
			
		}
		else if(strcmp(answer, "3")==0){
			char dummy2[MAX_INPUT];
			
			printf("-- A jelenlegi felvitt kerdesek listaja --");
			printf("\n------------------------------------------\n");
			listQuestions();
			printf("\n------------------------------------------\n");
			printf("Melyik kerdest akarja torolni?\n");
			
			char sorszam = 0;
			
			scanf("%d", &sorszam);
			fgets(dummy2, MAX_INPUT, stdin);
			//if(sorszam == ) tudjon visszalepni ha akar
			deleteByNumber(sorszam);
			
		}
		else if(strcmp(answer, "4")==0){
			listQuestions();
		}
		else if(strcmp(answer, "5")==0){
			//printf("Kerdoiv veglegesitese.\n");
			char minimenu[MAX_INPUT];
			char cont[1];
			do{
				
				printf("Kerdest akarok veglegesiteni: 1 \n");
				printf("Kesz: E\n");

				scanf("%c", cont);
				fgets(minimenu, MAX_INPUT, stdin);
				
				if(strcmp(cont, "1")==0){
					
					printf("-- A jelenlegi felvitt kerdesek listaja --");
					printf("\n------------------------------------------\n");
					listQuestions();
					printf("\n------------------------------------------\n");
					printf("Melyik kerdest akarja vegelgesiteni?\n");
					char conte =0;
					scanf("%d", &conte);
					fgets(minimenu, MAX_INPUT, stdin);
					
					finalQ(conte);
					deleteByNumber(conte);
				}
				
			}
			while(strcmp(cont, "E") !=0);
		}
	}while(strcmp(answer, "X")!=0);

}
	
void newQuestion(struct question_body new){
		FILE * fileReadIn = fopen("QUESTIONS.txt", "a+");

		if (fileReadIn == NULL){perror("File opening error\n"); exit(1);}
		else{
			if (fprintf(fileReadIn, &new.theQuestion )< 0){
				printf("Hiba a fajlba iraskor!\n");
			}
			if (fprintf(fileReadIn, &new.answer_1 )< 0){
				printf("Hiba a fajlba iraskor!\n");
			}
			if (fprintf(fileReadIn, &new.answer_2 )< 0){
				printf("Hiba a fajlba iraskor!\n");
			}
			if (fprintf(fileReadIn, &new.answer_3 )< 0){
				printf("Hiba a fajlba iraskor!\n");
			}
			if (fprintf(fileReadIn, ido_formatum(new.recordDate) )< 0){
				printf("Hiba a fajlba iraskor!\n");
			}
			if (fprintf(fileReadIn, "\n" )< 0){
				printf("Hiba a fajlba iraskor!\n");
			}
			else{
				printf("felvitt adatok: %s|%s|%s|%s|%s\n",&new.theQuestion, &new.answer_1, new.answer_2, &new.answer_3, ido_formatum(new.recordDate));

			}
		}
		
		fclose(fileReadIn);
	}
	
void listQuestions(){
		FILE * fileReadIn = fopen("QUESTIONS.txt","r");
		char dummyLine[MAX_INPUT];
		int num=0;
		while(fgets(dummyLine,sizeof(dummyLine),fileReadIn)){
			
			if((num % 5)==0){
				printf("%d", (num/5));
				printf("\. ");
				printf("%s", dummyLine);
			}else{
				printf("- ");
				printf("%s", dummyLine);
			}
			++num;
		}
		printf("\n_____________________________\n");
		
	}
	
void deleteByNumber(int sorszam){
		FILE * fileReadIn = fopen("QUESTIONS.txt", "r");
		FILE * fileWriteOut = fopen("TEMP.txt", "w+");

		if (fileReadIn == NULL){perror("File opening error\n"); exit(1);}
		else{
			char dummyLine[MAX_INPUT];
			int num = 0;
			while(fgets(dummyLine,sizeof(dummyLine),fileReadIn)!=NULL){
				if(num == sorszam){
					printf("Az adat torolve\n");
					int u ;
					for(u=0;u<4;++u){
						fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					}
					num++;
				}
				else{
					//printf("Kerdes fgetje\n");
					if (fprintf(fileWriteOut, dummyLine )< 0){
					printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					//printf("elso valasz fgetje\n");
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					//printf("masodik valasz fgetje\n");
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					//printf("harmadik valasz fgetje\n");
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					//printf("Datum fgetje\n");
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					num++;
				}
				
			}
			
	
		}
		fclose(fileReadIn);
		fclose(fileWriteOut);
		
		if (remove("QUESTIONS.txt")!=0){
		printf("Hiba tortent a mentes soran! #1");
		}
		else{
			if (rename("TEMP.txt", "QUESTIONS.txt")!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes!\n");
			}
		}
		
	}
	
void editByNumber(int sorszam){
		FILE * fileReadIn = fopen("QUESTIONS.txt", "r");
		FILE * fileWriteOut = fopen("TEMP.txt", "w+");
		
		char dummyLine[MAX_INPUT];
			int num = 0;
			while(fgets(dummyLine,sizeof(dummyLine),fileReadIn)!=NULL){
				if(num == sorszam){
					int u ;
					for(u=0;u<4;++u){
						fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					}
						struct question_body new;
						printf("Kerdes javitasa:\n");
						fgets(&new.theQuestion, MAX_INPUT, stdin);

						printf("Elso valasz javitasa:\n");
						fgets(&new.answer_1, MAX_INPUT, stdin);
						
						printf("Masodik valasz javitasa:\n");
						fgets(&new.answer_2, MAX_INPUT, stdin);
						
						printf("Harmadik valasz javitasa:\n");
						fgets(&new.answer_3, MAX_INPUT, stdin);
						new.recordDate = time(NULL);
						
						if (fprintf(fileWriteOut, &new.theQuestion )< 0){
							printf("Hiba a fajlba iraskor!\n");
						}
						if (fprintf(fileWriteOut, &new.answer_1 )< 0){
							printf("Hiba a fajlba iraskor!\n");
						}
						if (fprintf(fileWriteOut, &new.answer_2 )< 0){
							printf("Hiba a fajlba iraskor!\n");
						}
						if (fprintf(fileWriteOut, &new.answer_3 )< 0){
							printf("Hiba a fajlba iraskor!\n");
						}
						if (fprintf(fileWriteOut, ido_formatum(new.recordDate) )< 0){
							printf("Hiba a fajlba iraskor!\n");
						}
						if (fprintf(fileWriteOut, "\n" )< 0){
							printf("Hiba a fajlba iraskor!\n");
						}
					num++;
				}
				else{
					if (fprintf(fileWriteOut, dummyLine )< 0){
					printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}

					num++;
				}
				
			}
		fclose(fileReadIn);
		fclose(fileWriteOut);
		
		if (remove("QUESTIONS.txt")!=0){
		printf("Hiba tortent a mentes soran! #1");
		}
		else{
			if (rename("TEMP.txt", "QUESTIONS.txt")!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes!\n");
			}
		}
		
	}
	
void finalQ(int sorszam){
		FILE * fileReadIn = fopen("QUESTIONS.txt", "r");
		FILE * fileWriteOut = fopen("FINAL_QUESTIONS.txt", "a+");
		printf("megnyitottam a fileokat\n");

		if (fileReadIn == NULL){perror("File opening error\n"); exit(1);}
		else{
			char dummyLine[MAX_INPUT];
			int num = 0;
			while(fgets(dummyLine,sizeof(dummyLine),fileReadIn)!=NULL){
				if(num == sorszam){
					//printf("Kerdes fgetje\n");
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					if (fprintf(fileWriteOut, dummyLine )< 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					num++;
				}
				else{
					printf("Az adat veglegesitve\n");
					int u ;
					for(u=0;u<4;++u){
						fgets(dummyLine,sizeof(dummyLine),fileReadIn);
					}
					num++;
				}	
			}
			printf("\n_____________________________\n");
		}
		fclose(fileReadIn);
		fclose(fileWriteOut);	
		
		//indul a szulo 
		
		int pipePtC[2];
			int pipeCtP[2];// unnamed pipe file descriptor array
			pid_t pid;
			char sz[100];	// char array for reading from pipe szulonek
			char gy[100];	//gyereknek
			
			   if (pipe(pipePtC) == -1) 
				{
				   perror("Hiba a 'Szulo ir gyereknek' pipe nyitaskor!");
				   exit(EXIT_FAILURE);
				}
				if (pipe(pipeCtP) == -1) 
				{
				   perror("Hiba a 'gyerek ir szulonek' pipe nyitaskor!");
				   exit(EXIT_FAILURE);
				}
			   pid = fork();	// creating parent-child processes
			   if (pid == -1) 
				{
				   perror("Fork hiba");
				   exit(EXIT_FAILURE);
				}

			   if (pid == 0) 
				{// child process
			   
				sleep(1);
			   
				close(pipePtC[1]);  //olvaso
				close(pipeCtP[0]);	//iro	
				
				write(pipeCtP[1], "Ready",6);
				
				printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
				int ij=0;
				read(pipePtC[0],&ij,sizeof(int));
				
				char str[15];
				sprintf(str, "%d", ij);
				
				printf("Gyerek olvasta uzenet: %s",str);
				
				printf("\n");
				close(pipePtC[0]); // finally we close the used read end
				} 
			   else 
				{    // szulo process 
					printf("Szulo indul!\n");
					close(pipePtC[0]);	//irni fog ezert lezarom az olvasot
					close(pipeCtP[1]);	//olvasni fog ezert lezarom az irot
					
					read(pipeCtP[0], sz, sizeof(sz));
					printf("A gyerek azt uzeni : %s \n", sz);
					
				   //random kerdes generalas
					srand(time(NULL));
					int r=rand()%10 + 10; 
					printf("Random number %d\n",r);
				   
					//ir a gyereknek
					write(pipePtC[1], &r, sizeof(int));
					
					close(pipePtC[1]); // Closing write descriptor 
					printf("Szulo beirta az adatokat a csobe!\n");
					fflush(NULL); 	// flushes all write buffers (not necessary)
					wait();		// waiting for child process (not necessary)
					// try it without wait()
					printf("Szulo befejezte!");	
			}
			exit(EXIT_SUCCESS);	// force exit, not necessary
		
		
	}
	
const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S",localtime(&t));
	return buff;
}
