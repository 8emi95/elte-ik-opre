#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h> //ctime
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int get_menu_choice(void);
int check_file(const char *fileName);
void new_questionList();
int question_counter();
void list_questions();
void change_question();
void delete_question();
void new_question();
void delete_questionList();
void mlsz();


struct Question
{
    char question[50];
    char id[2];
};

int main()
{
    int p;
    struct Question defaultQuestion;
    strcpy(defaultQuestion.question, "Question List");
	
    strcpy(defaultQuestion.id, "0");
    FILE *fptr = fopen(defaultQuestion.id, "w");
    if (fptr != NULL)
    {
        fclose(fptr);
    }
    int choice;
    do
    {
        choice = get_menu_choice();

        switch (choice)
        {
        case 1:
            new_questionList();
            break;
        case 2:
            list_questions();
            break;
        case 3:
            change_question();
            break;
        case 4:
            delete_question();
            break;
        case 5:
            new_question();
            break;
		case 6:
            mlsz();
            break;
        }
    } while (choice != 0);
	
	return 0;
}

int get_menu_choice(void)
{
    int selection = 0;

    do
    {
        printf("1 - Új kérdés sor hozzáadása");
        printf("\n2 - Kérdések listázása");
        printf("\n3 - Kérdés adatok módositása.");
        printf("\n4 - Kérdés törlése.");
        printf("\n5 - Új kérdés hozzáadása.");
		printf("\n6 - MLSZ Kérdések küldése.");
        printf("\n0 - Kilépés.");
        printf("\nVálasszon egy menüpontot: ");
        scanf("%d", &selection);

    } while ((selection < 0) || (selection > 7));
    return selection;
}

int check_file(const char *fileName)
{
    FILE *fptr = fopen(fileName, "r");
    if (fptr != NULL)
    {
        fclose(fptr);
        return 1;
    }
    return 0;
}

void new_question()
{
    char question[50];
    printf("Adja meg a kérdést: ");
    scanf("%s", &question);
	
	printf("Adja meg a válasz lehetőségeket: ");
	char answer1[10];
	char answer2[10];
	char answer3[10];
	scanf("%s", &answer1);
	scanf("%s", &answer2);
	scanf("%s", &answer3);

    char id[2];
    printf("Adja meg a kérdéssor sorszámát: ");
    scanf("%s", &id);

    time_t current_time;
    char *c_time_string;

    current_time = time(NULL);

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    if (check_file(id) == 1)
    {
        FILE *f = fopen(id, "a");
        fprintf(f, "%s\n", question);
		fprintf(f, "%s\n", answer1);
		fprintf(f, "%s\n", answer2);
		fprintf(f, "%s\n", answer3);
        fprintf(f, "%s", c_time_string);
        fclose(f);
    }
    else
    {
        printf("Nincs ilyen esemény. \n");
    };

}


void list_questions()
{
    char id[2];
    printf("Adja meg a kérdéssor sorszámát: ");
    scanf("%s", &id);
    FILE *f;
    f = fopen(id, "r");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }
    char line[50];
    while (fgets(line, sizeof(line), f))
    {
        printf("%s", line);
    }
    printf("\n");
    fclose(f);
}

void delete_question()
{
    char question[50];
    printf("Adja meg a kérdést: ");
    scanf("%s", &question);
    strcat(question, "\n");

    char id[2];
    printf("Adja meg a kérdéssor sorszámát: ");
    scanf("%s", &id);

    FILE *f;
    f = fopen(id, "r");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    FILE *f_new;
    f_new = fopen("temp", "w");
    if (f_new == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    char line1[50];
    char line2[10];
	char line3[10];
	char line4[10];
	char line5[50];
    while (!feof(f))
    {
        fgets(line1, sizeof(line1), f);
        fgets(line2, sizeof(line2), f);
		fgets(line3, sizeof(line2), f);
		fgets(line4, sizeof(line2), f);
		fgets(line5, sizeof(line2), f);
        if (strcmp(line1, question) != 0 && strcmp(line1, "\n") != 0)
        {
            fprintf(f_new, "%s", line1);
            fprintf(f_new, "%s", line2);
			fprintf(f_new, "%s", line3);
            fprintf(f_new, "%s", line4);
			fprintf(f_new, "%s", line5);
        }
    }
    fclose(f);
    remove(id);
    fclose(f_new);
    rename("temp", id);
}

void change_question()
{
    char question[50];
    printf("Adja meg a módosítandó kérdést: ");
    scanf("%s", &question);
    strcat(question, "\n");

    char id[2];
    printf("Adja meg a kérdéssor sorszámát: ");
    scanf("%s", &id);

    char new_question[50];
    printf("Adja meg az új kérdést: ");
    scanf("%s", &new_question);
	
	printf("Adja meg a válasz lehetőségeket: ");
	char answer1[10];
	char answer2[10];
	char answer3[10];
	scanf("%s", &answer1);
	scanf("%s", &answer2);
	scanf("%s", &answer3);

    time_t current_time;
    char *c_time_string;

    current_time = time(NULL);
    c_time_string = ctime(&current_time);

    FILE *f;
    f = fopen(id, "r");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    FILE *f_new;
    f_new = fopen("temp", "w");
    if (f_new == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }

    char line1[50];
    char line2[10];
	char line3[10];
	char line4[10];
	char line5[50];
    while (!feof(f))
    {
        fgets(line1, sizeof(line1), f);
        fgets(line2, sizeof(line2), f);
		fgets(line3, sizeof(line2), f);
		fgets(line4, sizeof(line2), f);
		fgets(line5, sizeof(line2), f);
        if (strcmp(line1, question) != 0 && strcmp(line1, "\n") != 0)
        {
            fprintf(f_new, "%s", line1);
            fprintf(f_new, "%s", line2);
			fprintf(f_new, "%s", line3);
            fprintf(f_new, "%s", line4);
			fprintf(f_new, "%s", line5);
        }
        if (strcmp(line1, question) == 0 && strcmp(line1, "\n") != 0)
        {
            fprintf(f_new, "%s\n", new_question);
            fprintf(f_new, "%s", answer1);
			fprintf(f_new, "%s", answer2);
			fprintf(f_new, "%s", answer3);
			fprintf(f_new, "%s", c_time_string);
			
			
        }
    }
    fclose(f);
    remove(id);
    fclose(f_new);
    rename("temp", id);
}

void new_questionList()
{
    printf("Mi legyen a kérdéssor sorszáma? ");
    char id[2];
    scanf("%s", &id);
    FILE *f;
    f = fopen(id, "w");
    if (f == NULL)
    {
        perror("File opening error\n");
        exit(1);
    }
}

void delete_questionList()
{
    printf("Melyik sorszámú kérdéssort törli? ");
    char id[2];
    scanf("%s", &id);
    remove(id);
}

/*
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdetett a labdajátékok felvirágoztatására! 
Első lépésként megalkotjuk azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4 választási 
lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. Miután véglegesítettük 
a kérdéseket az MLSZ (szülő) kérdezőbiztost(gyerek) küld a paraméterként megadott labdajáték centrumba, városba.

Az MLSZ véletlenszerűen kiválaszt 3 kérdést, amit csővezetéken kiküld a kérdezőbiztosnak azután, hogy az bejelentkezett az MLSZ-hez, 
hogy készen áll a feladat végrehajtására. A kérdezőbiztos véletlenszerűen választ 10 és 20 közti konzultálni kívánó polgárt, 
akik kitöltik a kérdőívet (véletlenszerűen). Az eredményt visszaküldi csővezetéken az MLSZ-be, majd a kérdezőbiztos befejezi a konzultációt. 
Az MLSZ fogadja az eredményt és a képernyőre írja a konzultáció eredményét.
*/


int signalFlag = 0;

void handler(int signumber)
{
	if(signumber==2){
		printf("Interrupted by signal!\n");
		exit(-1);
	}else{
   signalFlag = signumber;
	}
}

int countLines(char fileName[]){//,pid_t pid){
	int status;
	FILE* myfile = fopen(fileName, "r");
	if (myfile == NULL)
	{
		printf("Error opening file!\n");
		//kill(pid,SIGINT);
		//wait(&status);
		exit(-1);
	}
	int ch, lines = 0;

	do 
	{
		ch = fgetc(myfile);
		if(ch == '\n')
			lines++;
	} while (ch != EOF);

	if(ch != '\n' && lines != 0) 
		lines++;

	fclose(myfile);

	return lines-1;
}

void mlsz(){
	int status;
	srand(time(NULL));
	
	signal(SIGTERM, handler);
	signal(SIGUSR1, handler);
	signal(SIGINT, handler);

	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;

	char sz[150];  // char array for reading from pipe

	if (pipe(pipefd) == -1) 
	{
	perror("Error opening the pipe!");
	exit(EXIT_FAILURE);
	}
	//int p = countLines("2")/5;
	
	pid =fork(); //forks make a copy of variables
	if (pid<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (pid>0) //the parent process, it can see the returning value of fork - the child variable!
	{
		int i=0;
		int kerdesek[3]={0,0,0};
		while(i!=3){
			int r=(rand()%(countLines("2")/5))+1;
			if(r == 0 || kerdesek[0] == r || kerdesek[1] == r || kerdesek[2] == r) continue;
			kerdesek[i]=r;
			i++;
		}
		
		FILE *f = fopen("2","r");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			kill(pid,SIGINT);
		    wait(&status);
		    exit(-1);
		}
		
		char line1[15][50];
		
		
		char collector[50];
		i=1;
		int k = 5;
		while (!feof(f))
		{
			if(kerdesek[0] == i || kerdesek[1] == i || kerdesek[2] == i){
				fgets(line1[k-5], 50, f);
				fgets(line1[k-4], 50, f);
				fgets(line1[k-3], 50, f);
				fgets(line1[k-2], 50, f);
				fgets(line1[k-1], 50, f);
			}else{
				fgets(collector, sizeof(collector), f);
				collector[0] = '\0';
				fgets(collector, sizeof(collector), f);
				collector[0] = '\0';
				fgets(collector, sizeof(collector), f);
				collector[0] = '\0';
				fgets(collector, sizeof(collector), f);
				collector[0] = '\0';
				fgets(collector, sizeof(collector), f);
			}
			i++;
			k += 5;
		}
		
		write(pipefd[1], line1, sizeof line1);
		char hanytmp[10];
		int hanyember;
		read(pipefd[0], hanytmp, 10);
		hanyember=atoi(hanytmp);
		char szoveg[50];
		for(i=0;i<hanyember;i++){
			
			read(pipefd[0], szoveg, sizeof szoveg);
			printf("%d. ember:\t",szoveg);
			read(pipefd[0], szoveg, sizeof szoveg);
			printf("%s\t",szoveg);
			read(pipefd[0], szoveg, sizeof szoveg);
			printf("%s\t",szoveg);
			read(pipefd[0], szoveg, sizeof szoveg);
			printf("%s\n",szoveg);
			
		}
		
		
		
		close(pipefd[0]);
		close(pipefd[1]);
		fflush(NULL); 	// flushes all write buffers (not necessary)
		wait(NULL);		// waiting for child process (not necessary)
				// try it without wait()
		printf("Szulo befejezte!\n");	
		
	}
	else //child process
	{
		char kapottKerdesek[15][50];;
		read(pipefd[0], kapottKerdesek, sizeof kapottKerdesek);
		
		int hanyember=(rand()%11)+10;
		int i;
		for( i = 0; i < hanyember; ++i){
			write(pipefd[1],i,sizeof i);
			write(pipefd[1],kapottKerdesek[(rand()%3)+1],50);
			write(pipefd[1],kapottKerdesek[(rand()%3)+6],50);
			write(pipefd[1],kapottKerdesek[(rand()%3)+11],50);
		}
		close(pipefd[0]);
		close(pipefd[1]);		// finally we close the used read end
		fflush(NULL);
		printf("(Gyerek befejezte!)\n");
	}
}
