#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

int countLines(char *filename);
void swap(int* a, int* b);
void handler(int signumber);

int main(int argc, char *argv[]){
	signal(SIGTERM,handler);
	srand(time(NULL));
	
	/* Csovezetek letrehozasa */
	int MyPipeFiel[2];

	int MyPipe = pipe(MyPipeFiel);
	if (MyPipe < 0)
	{
		perror("\nCsovezetek megnyitasa sikertelen!\n");
		exit(-1);
	}

	/* Gyerek letrehozasa */
	pid_t child = fork();
	if (child < 0){
		perror("\nGyerek letrehozasa sikertelen!\n");
		exit(-1);
	}

	/* Szulo folyamat */
	if (0 != child){
		printf("SZULO: Varok egy kerdezobiztosra\n");
		sleep(1);
        kill(child, SIGTERM);
		
		pause();
		printf("SZULO: Kerdezobiztos bejelentkezett, dolgozz %s centrumban/varosban!\n", argv[1]);

		/* Kerdesek helyenek meghatarozasa */
		FILE *fp1;
		fp1 = fopen("questions", "r");
		int questNums = countLines("questions") / 6;
		int whereIsQuestion[questNums];
		fclose(fp1);

		int i;
		int j;
		for (i = 0; i < questNums; i++){
			whereIsQuestion[i] = i * 6;
		}

		/* Random kerdesek generalasa */
		int randQuestion[3];
		for (i = 0; i < 3; i++){
			randQuestion[i] = rand() % questNums;
			int temp = i-1;
			while(temp >= 0){
				if(randQuestion[i] == randQuestion[temp]){
					randQuestion[i] = rand() % questNums;
					++temp;
				}
				--temp;
			} 
		}

		if (randQuestion[0] > randQuestion[2]) swap(&randQuestion[0], &randQuestion[2]);
		if (randQuestion[0] > randQuestion[1]) swap(&randQuestion[0], &randQuestion[1]);
		if (randQuestion[1] > randQuestion[2]) swap(&randQuestion[1], &randQuestion[2]);

		fp1 = fopen("questions", "r");
		if (fp1 == NULL){
			printf("\nHiba a fajl megnyitasakor\n");
			exit(1);
		}

		/* Kerdesek csobe irasa */
		char qa[3][220];
		char line[100];
		for(i = 0; i < 3; i++){
			j = 0;
			while (fgets(line, sizeof(line), fp1) && j <= whereIsQuestion[randQuestion[i]] + 1){
				++j;
				if (j == whereIsQuestion[randQuestion[i]] + 1){
					strcpy(qa[i], line);
					fgets(line, sizeof(line), fp1);
					strcat(qa[i], line);
					fgets(line, sizeof(line), fp1);
					strcat(qa[i], line);
					fgets(line, sizeof(line), fp1);
					strcat(qa[i], line);
					fgets(line, sizeof(line), fp1);
					strcat(qa[i], line);
				}
			}
		}
		fclose(fp1);
			
		write(MyPipeFiel[1], qa, sizeof(qa));
		sleep(1);
		kill(child, SIGTERM);
		
		pause();

		/* Valaszok kezelese */
		int kitolto;
		read(MyPipeFiel[0], &kitolto, sizeof(kitolto));
		
		int result[3][kitolto];
		read(MyPipeFiel[0], result, sizeof(result));
		printf("SZULO: Megkaptam a válaszokat:\n");

		for(i = 0; i < 3; i++){
			printf("%d. kerdes:\n", i+1);
			for(j = 0; j < kitolto; j++){
				printf("%d. kitolto: %d\n", j+1, result[i][j]);
			}
		printf("\n");
		} 	
	}
	/* Gyerek folyamat */
	else{
		pause();
		printf("GYEREK: Bejelentkeztem az MLSZ-hez\n");
		sleep(1);
		kill(getppid(), SIGTERM);

		pause();
		printf("GYEREK: Elkezdek dolgozni %s centrumban/varosban\n", argv[1]);

		/* Valaszolok generalasa */
		int people = rand() % 11 + 10;
		int answr[3][people + 1];

		char buffer[3][220];
		read(MyPipeFiel[0], buffer, sizeof(buffer));
		
		/* Valaszok generalasa */
		int i;
		int j;
		for(i = 0; i < 3; i++){
			int count = 0;
			for(j = 0; j < 220; j++){
				if(buffer[i][j] == '0' && buffer[i][j+1] == '0'){
					count++;
				}
			}
			for(j = 0; j <= people; j++){
				answr[i][j] = rand() % (4 - count) + 1;
			}
		}

		/* Valaszok visszaadasa */
		printf("GYEREK: Kitoltettem a kerdoivet, befejeztem a munkat\n");
		write(MyPipeFiel[1], &people, sizeof(people));
		write(MyPipeFiel[1], answr, sizeof(answr));
		
		sleep(1);
		kill(getppid(), SIGTERM);
	}
	return 0;
}


int countLines(char *filename){
	FILE *fp = fopen(filename, "r");

	char c;
	int lines = 1;

	while ((c = fgetc(fp)))
	{
		if (c == EOF)
			break;
		if (c == '\n')
		{
			lines++;
		}
	}
	fclose(fp);

	return lines;
}

void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}


void handler(int signumber)
{
    //printf("%i. szignal megerkezett\n", signumber);
}